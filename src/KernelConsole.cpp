#include "../h/KernelConsole.hpp"

KernelConsole* KernelConsole::getInstance(){
    static KernelConsole instance;
    return &instance;
}

KernelConsole::KernelConsole() {
    inputBuffer = new KernelBuffer();
    outputBuffer = new KernelBuffer();
    outputThread = nullptr;
}

uint64 KernelConsole::readStatus() {
    return *((volatile uint8*)CONSOLE_STATUS);
}

char KernelConsole::readRxData() {
    return *((volatile char*)CONSOLE_RX_DATA);
}

void KernelConsole::writeTxData(char c) {
    *((volatile char*)CONSOLE_TX_DATA) = c;
}


//Za syscall_c
int KernelConsole::putc(char c) {
    if (outputBuffer == nullptr) {
        return -1;
    }

    outputBuffer->append(c);
    return 0;
}
char KernelConsole::getc() {
    if (inputBuffer == nullptr) {
        return '-';
    }

    return inputBuffer->take();
}

void KernelConsole::ConsoleInterrupt() {
    if (inputBuffer == nullptr) {
        return;
    }

    while ((readStatus() & CONSOLE_RX_STATUS_BIT) != 0) {
        char c = readRxData();
        if (!inputBuffer->full()) {
            inputBuffer->append(c);
        }
    }
}

void KernelConsole::outputThreadBody(void* arg) {

    while (true) {
        if (KernelConsole::getInstance()->outputBuffer == nullptr) {
            TCB::dispatch();
            continue;
        }

        char c = KernelConsole::getInstance()->outputBuffer->take();

        while ((readStatus() & CONSOLE_TX_STATUS_BIT) == 0) {
            TCB::dispatch();
        }
        writeTxData(c);
    }
}

TCB *KernelConsole::startOutputThread() {
    if (outputThread != nullptr) {
        return outputThread;
    }

    void* stack = MemoryAllocator::getInstance()->mem_alloc(DEFAULT_STACK_SIZE);
    if (stack == nullptr) {
        return nullptr;
    }

    outputThread = TCB::createThread(outputThreadBody, this, stack, true);

    if (outputThread == nullptr) {
        MemoryAllocator::getInstance()->mem_free(stack);
    }

    return outputThread;
}

void KernelConsole::clearBufferIllegalError() {
    if (outputBuffer == nullptr) return;

    char c = '\0';
    do {
        c = outputBuffer->takeIllegalError();
        while ((readStatus() & CONSOLE_TX_STATUS_BIT) == 0){}

        writeTxData(c);
    }while (c != '\0');
}