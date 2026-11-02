#ifndef PROJECT_BASE_V1_1_BUFFER_HPP
#define PROJECT_BASE_V1_1_BUFFER_HPP

#include "../h/KernelSemaphore.hpp"
#include "../lib/hw.h"

constexpr uint64 N = 1024;
class KernelBuffer {
public:
    KernelBuffer();
    ~KernelBuffer();

    static void* operator new(size_t size);
    static void operator delete(void* ptr);
    static void operator delete(void* ptr, size_t size);

    void append(char c);
    char take();

    uint64 get_capacity() const {return capacity;}
    bool full() const {return capacity == N;}

private:
    uint64 capacity, head, tail;
    //Vidi da li sme da bude obican niz a ne pointer, zbog alokacije niza
    char buffer[N];
    KernelSemaphore *mutex;
    KernelSemaphore *spaceAvailable, *itemAvailable;

    friend class KernelConsole;
    char takeIllegalError();
};


#endif //PROJECT_BASE_V1_1_BUFFER_HPP
