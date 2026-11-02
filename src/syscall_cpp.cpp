#include "../h/syscall_cpp.hpp"
#include "../h/syscall_c.hpp"

//Thread___________________________________________________________________________________________
Thread::Thread (void (*body)(void*), void* arg) : myHandle(nullptr), body(body), arg(arg) {}
Thread::Thread() : myHandle(nullptr), body(wrapperThread), arg(this) {}

Thread::~Thread() {
    //delete (void*)myHandle;
}

void Thread::wrapperThread(void *thread) {
    if (thread) {
        ((Thread*)thread)->run();
    }
}

int Thread::start() {
    return thread_create(&myHandle, body, arg);
}

void Thread::dispatch() {
    thread_dispatch();
}

int Thread::sleep(time_t_ time) {
    return time_sleep(time);
}

//Semaphore________________________________________________________________________________________
Semaphore::Semaphore(unsigned init) {
    sem_open(&myHandle, init);
}
Semaphore::~Semaphore() {
    sem_close(myHandle);;
}

int Semaphore::wait() {
    return sem_wait(myHandle);
}

int Semaphore::signal() {
    return sem_signal(myHandle);
}

//PeriodicThread___________________________________________________________________________________
void PeriodicThread::terminate() {
    period = 0;
}

PeriodicThread::PeriodicThread(time_t_ period) : period(period) {}

void PeriodicThread::periodicActivation(){}

void PeriodicThread::run() {
    while (period) {
        //Vidi da li treba prvo da spava pa onda da se aktivira ili obrnuto
        periodicActivation();
        sleep(period);
    }
}

char Console::getc() {
    return ::getc();
}
void Console::putc(char c) {
    return ::putc(c);
}
