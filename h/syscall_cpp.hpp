#ifndef PROJECT_BASE_V1_1_SYSCALL_CPP_HPP
#define PROJECT_BASE_V1_1_SYSCALL_CPP_HPP

#include "../h/syscall_c.hpp"

class Thread {
public:
    Thread (void (*body)(void*), void* arg);
    virtual ~Thread ();

    static void wrapperThread(void *thread);

    int start();

    static void dispatch();
    static int sleep(time_t_);

protected:
    Thread();
    virtual void run(){}

private:
    thread_t myHandle;
    void (*body)(void*);
    void* arg;
};

class Semaphore {
public:
    Semaphore(unsigned init = 1);
    virtual ~Semaphore();

    int wait();
    int signal();

private:
    sem_t myHandle;
};

class PeriodicThread : public Thread {
public:
    void terminate();

protected:
    PeriodicThread(time_t period);
    virtual void periodicActivation();
    virtual void run() override;

private:
    time_t period;
};

class Console {
public:
    static char getc();
    static void putc(char);
};


#endif //PROJECT_BASE_V1_1_SYSCALL_CPP_HPP
