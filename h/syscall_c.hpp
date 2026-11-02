#ifndef PROJECT_BASE_V1_1_SYSCALL_C_HPP
#define PROJECT_BASE_V1_1_SYSCALL_C_HPP

#include "../lib/hw.h"
enum RegisterIndex {
    REG_A0 = 10,
    REG_A1 = 11,
    REG_A2 = 12,
    REG_A3 = 13,
    REG_A4 = 14
};

enum userEnvironmentCalls {
    MEM_ALLOC = 0x01,
    MEM_FREE = 0x02,
    THREAD_CREATE = 0x11,
    THREAD_EXIT = 0x12,
    THREAD_DISPATCH = 0x13,
    SEM_OPEN = 0x21,
    SEM_CLOSE = 0x22,
    SEM_WAIT = 0x23,
    SEM_SIGNAL = 0x24,
    SEM_WAIT_N = 0x25,
    SEM_SIGNAL_N = 0x26,
    TIME_SLEEP = 0x31,
    GETC = 0x41,
    PUTC = 0x42
};

//Memory Allocator_____________________________________________________________________
//0x01
void* mem_alloc(size_t size);

//0x02
int mem_free(void* ptr);

//Thread_______________________________________________________________________________
//0x11
class _thread;
typedef _thread* thread_t;

int thread_create (
    thread_t* handle,
    void(*start_routine) (void*),
    void* arg
);

//0x12
int thread_exit();

//0x13
void thread_dispatch();

//Semaphore____________________________________________________________________________
//0x21
class _sem;
typedef _sem* sem_t;

int sem_open (
    sem_t* handle,
    unsigned init
);

//0x22
int sem_close (sem_t handle);

//0x23
int sem_wait(sem_t id);

//0x24
int sem_signal(sem_t id);

//0x25
int sem_wait_n(sem_t id, unsigned n);

//0x26
int sem_signal_n(sem_t id, unsigned n);

//Thread Sleep_________________________________________________________________________
//0x31
typedef unsigned long time_t;
typedef unsigned long time_t_;
int time_sleep(time_t time);

//Console______________________________________________________________________________
//0x41
const int EOF = -1;

char getc();

//0x42
void putc(char c);

#endif //PROJECT_BASE_V1_1_SYSCALL_C_HPP
