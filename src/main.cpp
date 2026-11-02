#include "../h/KernelConsole.hpp"
#include "../lib/hw.h"
#include "../h/syscall_cpp.hpp"
#include "../h/TCB.hpp"
#include "../h/RiscV.hpp"
#include "../h/Print.hpp"
#include "../h/MemoryAllocator.hpp"
#include "../h/KernelSemaphore.hpp"

extern void userMain();

static void userMainWrapper(void* arg) {
    userMain();

    sem_t semaphore = (sem_t)arg;
    sem_signal(semaphore);

}

static void idleWrapper(void*) {
    while (true) {
        thread_dispatch();
    }
}

int main() {
    RiscV::w_stvec((uint64)&RiscV::supervisorTrap);

    Scheduler scheduler;

    TCB::running = TCB::createThread(nullptr, nullptr, nullptr, true);

    KernelConsole::getInstance()->startOutputThread();

    RiscV::ms_sstatus(RiscV::SSTATUS_SIE);

    //thread_t idleThread = nullptr;
    //thread_create(&idleThread, idleWrapper, nullptr);
    void* idleStack = MemoryAllocator::getInstance()->mem_alloc(DEFAULT_STACK_SIZE);
    TCB::createThread(idleWrapper, nullptr, idleStack, true);

    KernelSemaphore* userThreadSemaphore = new KernelSemaphore(0);

    thread_t userThread = nullptr;
    thread_create(&userThread, userMainWrapper, userThreadSemaphore);

    userThreadSemaphore->wait();

    KernelPrintString("Kernel finshed\n");

    delete (TCB*)userThread;
    delete userThreadSemaphore;
    //delete TCB::running;

    return 0;
}