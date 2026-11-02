#include "../h/syscall_c.hpp"

//Memory Allocator_____________________________________________________________________
//0x01
void* mem_alloc(size_t size) {
    if (size <= 0) return nullptr;

    size_t sizeInBlocks = (size + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;

    __asm__ volatile("mv a1, %0" : : "r"(sizeInBlocks));
    __asm__ volatile("mv a0, %0" : : "r"(MEM_ALLOC));

    __asm__ volatile("ecall");

    void* ret;
    //mozda volatile
    __asm__ volatile("mv %0, a0" : "=r"(ret));

    return ret;
}

//0x02
int mem_free(void* ptr) {
    if (!ptr) return -1;

    __asm__ volatile("mv a1, %0" : : "r"(ptr));
    __asm__ volatile("mv a0, %0" : : "r"(MEM_FREE));

    __asm__ volatile("ecall");

    size_t ret;
    //mozda volatile
    __asm__ volatile("mv %0, a0" : "=r"(ret));

    return ret;
}

//Thread_______________________________________________________________________________
//0x11
class _thread;
typedef _thread* thread_t;

int thread_create (thread_t* handle, void(*start_routine) (void*), void* arg) {
    if (!handle || !start_routine) return -1;

    void* stack = mem_alloc(DEFAULT_STACK_SIZE);
    if (!stack) return -2;
    __asm__ volatile("mv a4, %0" : : "r"(stack));
    __asm__ volatile("mv a3, %0" : : "r"(arg));
    __asm__ volatile("mv a2, %0" : : "r"(start_routine));
    __asm__ volatile("mv a1, %0" : : "r"(handle));
    __asm__ volatile("mv a0, %0" : : "r"(THREAD_CREATE));

    __asm__ volatile("ecall");

    int ret;
    //mozda volatile
    __asm__ volatile("mv %0, a0" : "=r"(ret));

    return ret;
}

//0x12
int thread_exit() {
    __asm__ volatile("mv a0, %0" : : "r"(THREAD_EXIT));

    __asm__ volatile("ecall");

    int ret;
    //mozda volatile
    __asm__ volatile("mv %0, a0" : "=r"(ret));

    return ret;
}

//0x13
void thread_dispatch() {
    __asm__ volatile("mv a0, %0" : : "r"(THREAD_DISPATCH));

    __asm__ volatile("ecall");
}

//Semaphore____________________________________________________________________________
//0x21
int sem_open (sem_t* handle, unsigned init) {
    __asm__ volatile("mv a2, %0" : : "r"(init));
    __asm__ volatile("mv a1, %0" : : "r"(handle));
    __asm__ volatile("mv a0, %0" : : "r"(SEM_OPEN));

    __asm__ volatile("ecall");

    int ret;
    //mozda volatile
    __asm__ volatile("mv %0, a0" : "=r"(ret));

    return ret;
}

//0x22
int sem_close (sem_t handle) {
    __asm__ volatile("mv a1, %0" : : "r"(handle));
    __asm__ volatile("mv a0, %0" : : "r"(SEM_CLOSE));

    __asm__ volatile("ecall");

    int ret;
    //mozda volatile
    __asm__ volatile("mv %0, a0" : "=r"(ret));

    return ret;
}

//0x23
int sem_wait(sem_t id) {
    __asm__ volatile("mv a1, %0" : : "r"(id));
    __asm__ volatile("mv a0, %0" : : "r"(SEM_WAIT));

    __asm__ volatile("ecall");

    int ret;
    //mozda volatile
    __asm__ volatile("mv %0, a0" : "=r"(ret));

    return ret;
}

//0x24
int sem_signal(sem_t id) {
    __asm__ volatile("mv a1, %0" : : "r"(id));
    __asm__ volatile("mv a0, %0" : : "r"(SEM_SIGNAL));

    __asm__ volatile("ecall");

    int ret;
    //mozda volatile
    __asm__ volatile("mv %0, a0" : "=r"(ret));

    return ret;
}

//0x25
int sem_wait_n(sem_t id, unsigned n) {
    __asm__ volatile("mv a2, %0" : : "r"(n));
    __asm__ volatile("mv a1, %0" : : "r"(id));
    __asm__ volatile("mv a0, %0" : : "r"(SEM_WAIT_N));

    __asm__ volatile("ecall");

    int ret;
    //mozda volatile
    __asm__ volatile("mv %0, a0" : "=r"(ret));

    return ret;
}

//0x26
int sem_signal_n(sem_t id, unsigned n) {
    __asm__ volatile("mv a2, %0" : : "r"(n));
    __asm__ volatile("mv a1, %0" : : "r"(id));
    __asm__ volatile("mv a0, %0" : : "r"(SEM_SIGNAL_N));

    __asm__ volatile("ecall");

    int ret;
    //mozda volatile
    __asm__ volatile("mv %0, a0" : "=r"(ret));

    return ret;
}

//Thread Sleep_________________________________________________________________________
//0x31
int time_sleep(time_t time) {
    __asm__ volatile("mv a1, %0" : : "r"(time));
    __asm__ volatile("mv a0, %0" : : "r"(TIME_SLEEP));

    __asm__ volatile("ecall");

    int ret;
    //mozda volatile
    __asm__ volatile("mv %0, a0" : "=r"(ret));

    return ret;
}

//Console______________________________________________________________________________
//0x41
char getc() {
    __asm__ volatile("mv a0, %0" : : "r"(GETC));

    __asm__ volatile("ecall");

    int ret;
    //mozda volatile
    __asm__ volatile("mv %0, a0" : "=r"(ret));

    return ret;
}

//0x42
void putc(char c) {
    __asm__ volatile("mv a1, %0" : : "r"(c));
    __asm__ volatile("mv a0, %0" : : "r"(PUTC));

    __asm__ volatile("ecall");
}