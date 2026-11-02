#include "../h/Scheduler.hpp"

#include "../h/TCB.hpp"

Queue<TCB>* Scheduler::readyThreadQueue = nullptr;
SleepingThreadQueue* Scheduler::sleepingThreadQueue = nullptr;
uint64 Scheduler::time = 0;

Scheduler::Scheduler() {
    readyThreadQueue = new Queue<TCB>;
    sleepingThreadQueue = new SleepingThreadQueue;

    time = 0;
}

Scheduler::~Scheduler() {
    if (readyThreadQueue != nullptr) {
        delete readyThreadQueue;
        readyThreadQueue = nullptr;
    }

    if (sleepingThreadQueue != nullptr) {
        delete sleepingThreadQueue;
        sleepingThreadQueue = nullptr;
    }
}

TCB* Scheduler::get(){
    return readyThreadQueue->removeFirst();
}

void Scheduler::put(TCB* tcb){
    //if (tcb->isFinished()) return;
    readyThreadQueue->addLast(tcb);
}

void Scheduler::putSleeping(TCB* tcb, uint64 timeToSleep) {
    sleepingThreadQueue->add(tcb, time + timeToSleep);
}

void Scheduler::awake() {
    while (sleepingThreadQueue->getNum() > 0 && sleepingThreadQueue->peekFirstTime() <= Scheduler::time) {
        TCB *awokenTCB = sleepingThreadQueue->remove();

        if (awokenTCB != nullptr) {
            awokenTCB->setBlocked(false);
            put(awokenTCB);
        }

        //Generalno nemoj da brises
        //delete awokenTCB;
    }
}

void Scheduler::tick() {
    time++;
}

uint64 Scheduler::getTime() {
    return time;
}
