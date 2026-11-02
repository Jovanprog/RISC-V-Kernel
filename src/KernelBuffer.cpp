#include "../h/KernelBuffer.hpp"
KernelBuffer::KernelBuffer() {
    head = tail = capacity = 0;
    mutex = new KernelSemaphore(1);
    spaceAvailable = new KernelSemaphore(N);
    itemAvailable = new KernelSemaphore(0);
}

KernelBuffer::~KernelBuffer() {
    delete mutex;
    delete spaceAvailable;
    delete itemAvailable;

    mutex = nullptr;
    spaceAvailable = nullptr;
    itemAvailable = nullptr;
}

void KernelBuffer::append(char c){
    if (mutex == nullptr || spaceAvailable == nullptr || itemAvailable == nullptr) {
        return;
    }

    spaceAvailable->wait();
    uint64 oldSstatus = RiscV::r_sstatus();
    RiscV::mc_sstatus(RiscV::SSTATUS_SIE);
    mutex->wait();
    buffer[tail] = c;
    tail = (tail+1) % N;
    capacity++;
    mutex->signal();
    RiscV::w_sstatus(oldSstatus);
    itemAvailable->signal();
}

char KernelBuffer::take(){
    if (mutex == nullptr || spaceAvailable == nullptr || itemAvailable == nullptr) {
        return '\0';
    }
    itemAvailable->wait();
    uint64 oldSstatus = RiscV::r_sstatus();
    RiscV::mc_sstatus(RiscV::SSTATUS_SIE);
    mutex->wait();
    char c = buffer[head];
    head = (head+1) % N;
    capacity--;
    mutex->signal();
    RiscV::w_sstatus(oldSstatus);
    spaceAvailable->signal();
    return c;
}

char KernelBuffer::takeIllegalError() {
    if (capacity == 0) return '\0';

    char c = buffer[head];
    head = (head + 1) % N;
    capacity--;

    return c;
}

void* KernelBuffer::operator new(size_t size) {
    return MemoryAllocator::getInstance()->mem_alloc(size);
}

void KernelBuffer::operator delete(void* ptr){
    if (ptr != nullptr) {
        MemoryAllocator::getInstance()->mem_free(ptr);
    }
}

void KernelBuffer::operator delete(void* ptr, size_t size) {
    if (ptr != nullptr) {
        MemoryAllocator::getInstance()->mem_free(ptr);
    }
}