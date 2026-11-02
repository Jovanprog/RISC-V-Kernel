#ifndef PROJECT_BASE_V1_1_MEMORYALLOCATOR_HPP
#define PROJECT_BASE_V1_1_MEMORYALLOCATOR_HPP

#include "../lib/hw.h"

class MemoryAllocator {
public:
    static MemoryAllocator* getInstance();

    void* mem_alloc(size_t size);
    int mem_free(void* block);
private:
    enum State{
        FREE = 0,
        USED = 1
    };
    //MemBlock se koristi u dvostruko ulancanoj listi block-ova memorije
    struct MemBlock{
        size_t size;
        MemBlock* prev;
        MemBlock* next;
        State state;
    };

    MemBlock* memBlockHead;

    MemoryAllocator();

    static size_t alignMemBlock(size_t size);
    static size_t alignedHeader();
};


#endif //PROJECT_BASE_V1_1_MEMORYALLOCATOR_HPP
