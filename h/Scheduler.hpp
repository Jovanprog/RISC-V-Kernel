#ifndef PROJECT_BASE_V1_1_SCHEDULER_HPP
#define PROJECT_BASE_V1_1_SCHEDULER_HPP

#include "../h/Queue.hpp"
#include "../h/SleepingThreadQueue.hpp"
#include "../lib/hw.h"

//Dodaj listu sleepingThreads
class TCB;

class Scheduler {
public:

    Scheduler();
    ~Scheduler();

    static TCB* get();
    static void put(TCB* tcb);

    static void putSleeping(TCB* tcb, uint64 time);
    static void awake();

    static void tick();
    static uint64 getTime();
private:
    static Queue<TCB>* readyThreadQueue;
    static SleepingThreadQueue* sleepingThreadQueue;

    static uint64 time;
};


#endif //PROJECT_BASE_V1_1_SCHEDULER_HPP
