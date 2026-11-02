#ifndef PROJECT_BASE_V1_1_SEMAPHORE_HPP
#define PROJECT_BASE_V1_1_SEMAPHORE_HPP

#include "../lib/hw.h"
#include "../h/Queue.hpp"
#include "../h/TCB.hpp"
#include  "../h/MemoryAllocator.hpp"

class KernelSemaphore {
public:
    static void* operator new(size_t size);
    static void operator delete(void* ptr);

    KernelSemaphore(uint64 init=1);
    KernelSemaphore(const KernelSemaphore&) = delete;
    KernelSemaphore& operator=(const KernelSemaphore&) = delete;
    ~KernelSemaphore();

    int wait();
    int signal();
    int wait_n(unsigned n);
    int signal_n(unsigned n);
    int close();

    uint64 value() const {return val;}
    bool isDeleted() const {return deleted;}

protected:
    void block(unsigned n);
    void unblock();

private:
    struct BlockedThread {
        TCB* tcb;
        unsigned requested;

        BlockedThread(TCB* tcb, unsigned requested): tcb(tcb), requested(requested){}

        static void* operator new(size_t size){
            return MemoryAllocator::getInstance()->mem_alloc(size);
        }
        static void operator delete(void* ptr){
            if (ptr != nullptr) {
                MemoryAllocator::getInstance()->mem_free(ptr);
            }
        }

    };

    uint64 val;
    Queue<BlockedThread>* blocked;
    bool deleted;
};


#endif //PROJECT_BASE_V1_1_SEMAPHORE_HPP
