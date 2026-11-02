#ifndef PROJECT_BASE_V1_1_CONSOLE_HPP
#define PROJECT_BASE_V1_1_CONSOLE_HPP

#include "../h/KernelBuffer.hpp"

class KernelConsole {
public:
    static KernelConsole* getInstance();

    //Za syscall_c
    int putc(char c);
    char getc();

    //Za Console Interrupt
    void ConsoleInterrupt();

    //Za sinhroni ispis zbog illegal error-a
    void clearBufferIllegalError();

    TCB* startOutputThread();
    static void outputThreadBody(void* arg);
private:
    KernelBuffer* inputBuffer;
    KernelBuffer* outputBuffer;
    KernelConsole();
    TCB* outputThread;

    friend class KernelBuffer;

    static uint64 readStatus();
    static char readRxData();
    static void writeTxData(char c);
};


#endif //PROJECT_BASE_V1_1_CONSOLE_HPP
