#include "../h/TCB.hpp"

#include "../h/MemoryAllocator.hpp"
#include "../h/syscall_c.hpp"


TCB *TCB::running = nullptr;

uint64 TCB::timeSliceCounter = 0;

TCB::~TCB() {
    body = nullptr;
    args = nullptr;
    if (stack != nullptr) {
        MemoryAllocator::getInstance()->mem_free(stack);
        //delete stack;
        stack = nullptr;
    }
}

TCB *TCB::createThread(Body body, void* args, void* stack, bool kernelThread) {
    TCB* tcb = new TCB(body, args, stack, DEFAULT_TIME_SLICE, kernelThread);

    if (tcb != nullptr && body != nullptr) {
        Scheduler::put(tcb);
    }

    return tcb;
}

void TCB::yield() {
    __asm__ volatile("mv a0, %0" : : "r"(THREAD_DISPATCH));
    __asm__ volatile ("ecall");
}

void TCB::dispatch() {
    TCB *old = running;
    if (old != nullptr && !old->isFinished() && !old->isBlocked()) {
        //*((volatile uint8*)CONSOLE_TX_DATA) = 'A';
        Scheduler::put(old);
    }
    //else delete old;
    //Pogledaj da li treba da dodas stanja TCB-a ili je dovoljno da se zna u kom su
    //nizu, to jest ready ili sleeping
    running = Scheduler::get();

    if (running == nullptr) {
        //*((volatile uint8*)CONSOLE_TX_DATA) = '0';
        running = old;
        return;
    }

    //*((volatile uint8*)CONSOLE_TX_DATA) = 'N';

    timeSliceCounter = 0;
    if (old != nullptr && old != running) {
        //*((volatile uint8*)CONSOLE_TX_DATA) = 'C';

        TCB::contextSwitch(&old->context, &running->context);
    }
}

void TCB::threadWrapper() {
    if (!TCB::running->isKernelThread()) {
        RiscV::popSppSpie();
    }
    else {
        RiscV::ms_sstatus(RiscV::SSTATUS_SIE);
    }

    if (TCB::running != nullptr && TCB::running->body != nullptr) {
        TCB::running->body(TCB::running->args);
    }

    running->setFinished(true);
    TCB::yield();

    /*while (true) {
        TCB::dispatch();
    }*/
}

void TCB::exit() {
    if (TCB::running != nullptr) {
        TCB::running->setFinished(true);
        TCB::dispatch();
    }
}

void* TCB::operator new(size_t size) {
    return MemoryAllocator::getInstance()->mem_alloc(size);
}

void TCB::operator delete(void* ptr){
    if (ptr != nullptr) {
        MemoryAllocator::getInstance()->mem_free(ptr);
    }
}