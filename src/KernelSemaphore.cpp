#include "../h/KernelSemaphore.hpp"
#include "../h/MemoryAllocator.hpp"

/*static void debugPutc(char c) {
    *((volatile uint8*)CONSOLE_TX_DATA) = (uint8)c;
}*/
KernelSemaphore::KernelSemaphore(uint64 init) {
    val = init;
    blocked = new Queue<BlockedThread>();
    deleted = false;
}

KernelSemaphore::~KernelSemaphore() {
    close();
    delete blocked;
    blocked = nullptr;
}

int KernelSemaphore::close() {
    if (deleted) return -1;

    deleted = true;

    BlockedThread* temp = blocked->removeFirst();
    while (temp != nullptr) {
        TCB* tcb = temp->tcb;

        if (tcb != nullptr) {
            tcb->setSemaphoreResult(-1);
            tcb->setBlocked(false);
            Scheduler::put(tcb);
        }

        delete temp;
        temp = blocked->removeFirst();
    }

    return 0;
}

int KernelSemaphore::wait(){
    return wait_n(1);
}

int KernelSemaphore::signal(){
    return signal_n(1);
}

int KernelSemaphore::wait_n(unsigned n) {
    //debugPutc('w');
    if (deleted || blocked == nullptr || TCB::running == nullptr) {
        //debugPutc('!');
        return -1;
    }
    if (n == 0) {
        //debugPutc('z');
        return 0;
    }

    if (val >= n) {
        //debugPutc('v');
        val -= n;
        return 0;
    }

    //debugPutc('b');
    TCB* old = TCB::running;
    old->setSemaphoreResult(0);
    block(n);

    //debugPutc('a');
    return old->getSemaphoreResult();

}
int KernelSemaphore::signal_n(unsigned n) {
    if (deleted || blocked == nullptr) return -1;

    if (n == 0) return 0;

    val += n;
    unblock();
    return 0;
}


void KernelSemaphore::block(unsigned n){
    //debugPutc('x');
    if (!TCB::running || blocked == nullptr) {
        //debugPutc('?');
        return;
    }
    //if (TCB::running->isFinished()) return;
    BlockedThread* newBlockedThread = new BlockedThread(TCB::running, n);

    if (newBlockedThread == nullptr) {
        //debugPutc('m');
        TCB::running->setSemaphoreResult(-1);
        return;
    }

    TCB::running->setBlocked(true);
    blocked->addLast(newBlockedThread);

    //debugPutc('d');

    TCB::dispatch();

    //debugPutc('r');
}

void KernelSemaphore::unblock(){
    if (blocked == nullptr) return;
    while (true) {
       BlockedThread* first = blocked->peekFirst();

        if (first == nullptr || first->requested > val) return;

        first = blocked->removeFirst();
        val -= first->requested;

        TCB* newTcb = first->tcb;
        if (newTcb != nullptr) {
            newTcb->setSemaphoreResult(0);
            newTcb->setBlocked(false);
            Scheduler::put(newTcb);
        }

        delete first;
    }
}

void* KernelSemaphore::operator new(size_t size) {
    return MemoryAllocator::getInstance()->mem_alloc(size);
}

void KernelSemaphore::operator delete(void* ptr){
    if (ptr != nullptr) {
        MemoryAllocator::getInstance()->mem_free(ptr);
    }
}
