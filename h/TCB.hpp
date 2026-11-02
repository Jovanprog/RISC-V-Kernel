#ifndef PROJECT_BASE_V1_1_TCB_HPP
#define PROJECT_BASE_V1_1_TCB_HPP

#include "../lib/hw.h"
#include "../h/Scheduler.hpp"
#include "../h/RiscV.hpp"

class TCB {
public:
    static void* operator new(size_t size);
    static void operator delete(void* ptr);

    using Body = void (*)(void*);

    static TCB* createThread(Body body, void* args, void* stack, bool kernelThread = false);

    ~TCB();

    static TCB *running;

    static void yield();
    static void dispatch();

    bool isFinished() const {return finished;}
    void setFinished(bool value) {finished = value;}

    bool isBlocked() const {return blocked;}
    void setBlocked(bool value) {blocked = value;}

    uint64 getTimeSlice() const {return timeSlice;}

    int getSemaphoreResult() const { return semaphoreResult; }
    void setSemaphoreResult(int value) {semaphoreResult = value;}

    bool isKernelThread() const {return kernelThread;}

private:
    /*explicit TCB(Body body, void* args, void* stack, uint64 timeSlice = DEFAULT_TIME_SLICE) :
            body(body),
            args(args),
            stack(stack != nullptr ? (void*)((uint64)stack - DEFAULT_STACK_SIZE) : nullptr),
            context({body != nullptr ? (uint64) &threadWrapper : 0,
                    stack != nullptr ? (uint64) &stack: 0
                    }),
            timeSlice(timeSlice),
            finished(false),
            blocked(false),
            semaphoreResult(0){}*/

    explicit TCB(Body body, void* args, void* stack, uint64 timeSlice = DEFAULT_TIME_SLICE, bool kernelThread = false) :
            body(body),
            args(args),
            stack(stack),
            context({body != nullptr ? (uint64) &threadWrapper : 0,
                    stack != nullptr ? (uint64)stack + DEFAULT_STACK_SIZE: 0
                    }),
            timeSlice(timeSlice),
            finished(false),
            blocked(false),
            semaphoreResult(0),
            kernelThread(kernelThread){}

    struct Context {
        uint64 ra;
        uint64 sp;
    };

    Body body;
    void* args;
    void* stack;
    Context context;
    uint64 timeSlice;
    bool finished;
    bool blocked;
    int semaphoreResult;
    bool kernelThread;

    friend class KernelSemaphore;
    friend class RiscV;

    static void threadWrapper();

    static void contextSwitch(Context *oldContext, Context *runningContext);

    static void exit();

    static uint64 timeSliceCounter;
};


#endif //PROJECT_BASE_V1_1_TCB_HPP
