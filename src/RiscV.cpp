#include "../h/RiscV.hpp"

#include "../h/MemoryAllocator.hpp"
#include "../h/Print.hpp"
#include "../h/TCB.hpp"
#include "../h/KernelSemaphore.hpp"
#include "../h/KernelConsole.hpp"
#include "../h/syscall_c.hpp"

void RiscV::popSppSpie() {
    //Proveri masku
    mc_sstatus(SSTATUS_SPP);
    ms_sstatus(SSTATUS_SPIE);
    __asm__ volatile ("csrw sepc, ra");
    __asm__ volatile ("sret");
}

void RiscV::handleSupervisorTrap(uint64* regs){
    uint64 volatile scause = r_scause();
    uint64 volatile sepc = r_sepc();
    uint64 volatile sstatus = r_sstatus();

    if(scause == 0x0000000000000008UL || scause == 0x0000000000000009UL) {
        //Interrupt: no, cause code: environment call
        sepc += 4;

        uint64 a0 = regs[REG_A0];
        //__asm__ volatile("mv %0, a0" : "=r"(a0));

        switch (a0) {
            case MEM_ALLOC: {
                size_t numOfBlocks = (size_t)regs[REG_A1];

                if (numOfBlocks <= 0) {
                    regs[REG_A0] = (uint64)0;
                    break;
                }
                //__asm__ volatile("mv %0, a1" : "=r"(numOfBlocks));
                void* ret = MemoryAllocator::getInstance()->mem_alloc(numOfBlocks * MEM_BLOCK_SIZE);

                regs[REG_A0] = (uint64)ret;
                //__asm__ volatile("mv a0, %0" : : "r"(ret));
                break;
            }
            case MEM_FREE: {
                void* ptr = (void*)regs[REG_A1];
                //__asm__ volatile("mv %0, a1" : "=r"(ptr));
                int ret = MemoryAllocator::getInstance()->mem_free(ptr);

                regs[REG_A0] = (uint64)ret;
                //__asm__ volatile("mv a0, %0" : : "r"(ret));
                break;
            }
            case THREAD_CREATE: {
                thread_t* handle = (thread_t*)regs[REG_A1];
                TCB::Body body = (TCB::Body)regs[REG_A2];
                //void (*start_routine)(void*);
                void* arg = (void*)regs[REG_A3];
                void* stack = (void*)regs[REG_A4];

                /*__asm__ volatile("mv %0, a1" : "=r"(handle));
                __asm__ volatile("mv %0, a2" : "=r"(start_routine));
                __asm__ volatile("mv %0, a3" : "=r"(arg));
                __asm__ volatile("mv %0, a4" : "=r"(stack));*/

                if (handle == nullptr || body == nullptr || stack == nullptr) {
                    regs[REG_A0] = (uint64)-1;
                    break;
                }

                TCB* tcb = TCB::createThread(body, arg, stack);

                if (!tcb) {
                    regs[REG_A0] = (uint64)-2;
                }
                else {
                    *handle = (thread_t)tcb;
                    regs[REG_A0] = (uint64)0;
                }


                /**handle = (thread_t)tcb;

                if (handle) __asm__ volatile("mv a0, %0" : : "r"(0));
                else __asm__ volatile("mv a0, %0" : : "r"(-1));*/
                break;
            }
            case THREAD_EXIT: {
                TCB::exit();
                regs[REG_A0] = (uint64)0;
                break;
            }
            case THREAD_DISPATCH: {
                TCB::timeSliceCounter = 0;
                TCB::dispatch();
                regs[REG_A0] = (uint64)0;
                break;
            }
            case SEM_OPEN: {
                /*KernelSemaphore** handle;
                uint64 init;

                __asm__ volatile("mv %0, a1" : "=r"(handle));
                __asm__ volatile("mv %0, a2" : "=r"(init));

                *handle = new KernelSemaphore(init);

                if (handle) __asm__ volatile("mv a0, %0" : : "r"(0));
                else __asm__ volatile("mv a0, %0" : : "r"(-1));*/

                sem_t* handle = (sem_t*)regs[REG_A1];
                unsigned init = (unsigned)regs[REG_A2];

                if (handle == nullptr) {
                    regs[REG_A0] = (uint64)-1;
                    break;
                }

                KernelSemaphore* semaphore = new KernelSemaphore(init);

                if (semaphore == nullptr) {
                    regs[REG_A0] = (uint64)-2;
                }
                else {
                    *handle = (sem_t)semaphore;
                    regs[REG_A0] = (uint64)0;
                }
                break;
            }
            case SEM_CLOSE: {
                /*KernelSemaphore* handle;

                __asm__ volatile("mv %0, a1" : "=r"(handle));

                if (handle) {
                    //Vidi da li se zove destruktor;
                    //(handle)->~KernelSemaphore();
                    delete handle;
                    __asm__ volatile("mv a0, %0" : : "r"(0));
                }
                else __asm__ volatile("mv a0, %0" : : "r"(-1));*/
                KernelSemaphore* handle = (KernelSemaphore*)regs[REG_A1];

                if (handle == nullptr) {
                    regs[REG_A0] = (uint64)-1;
                    break;
                }

                int ret = handle->close();

                if (ret == 0) {
                    delete handle;
                }

                regs[REG_A0] = (uint64)ret;
                break;
            }
            case SEM_WAIT: {
                /*KernelSemaphore* handle;

                __asm__ volatile("mv %0, a1" : "=r"(handle));

                if (handle) {
                    int ret = handle->wait();
                    if (ret) __asm__ volatile("mv a0, %0" : : "r"(0));
                    else __asm__ volatile("mv a0, %0" : : "r"(-1));
                }
                else __asm__ volatile("mv a0, %0" : : "r"(-1));*/
                KernelSemaphore* handle = (KernelSemaphore*)regs[REG_A1];

                if (handle == nullptr) {
                    regs[REG_A0] = (uint64)-1;
                    break;
                }

                int ret = handle->wait();
                regs[REG_A0] = (uint64)ret;
                break;
            }
            case SEM_SIGNAL: {
                /*KernelSemaphore* handle;

                __asm__ volatile("mv %0, a1" : "=r"(handle));

                if (handle) {
                    int ret = handle->signal();
                    if (ret) __asm__ volatile("mv a0, %0" : : "r"(0));
                    else __asm__ volatile("mv a0, %0" : : "r"(-1));
                }
                else __asm__ volatile("mv a0, %0" : : "r"(-1));*/
                KernelSemaphore* handle = (KernelSemaphore*)regs[REG_A1];

                if (handle == nullptr) {
                    regs[REG_A0] = (uint64)-1;
                    break;
                }

                int ret = handle->signal();
                regs[REG_A0] = (uint64)ret;
                break;
            }
            case SEM_WAIT_N: {
                KernelSemaphore* handle = (KernelSemaphore*)regs[REG_A1];
                unsigned n = (unsigned)regs[REG_A2];

                if (handle == nullptr || n == 0) {
                    regs[REG_A0] = (uint64)-1;
                    break;
                }

                int ret = handle->wait_n(n);

                regs[REG_A0] = (uint64)ret;
                break;
            }
            case SEM_SIGNAL_N: {
                KernelSemaphore* handle = (KernelSemaphore*)regs[REG_A1];
                unsigned n = (unsigned)regs[REG_A2];

                if (handle == nullptr || n == 0) {
                    regs[REG_A0] = (uint64)-1;
                    break;
                }

                int ret = handle->signal_n(n);

                regs[REG_A0] = (uint64)ret;
                break;
            }
            case TIME_SLEEP: {
                /*time_t time;

                __asm__ volatile("mv %0, a1" : "=r"(time));

                Scheduler::putSleeping(TCB::running, time);
                TCB::timeSliceCounter = 0;
                TCB::dispatch();

                __asm__ volatile("mv a0, %0" : : "r"(0));*/
                time_t time = (time_t)regs[REG_A1];

                if (time <= 0) {
                    regs[REG_A0] = (uint64)0;
                    break;
                }

                if (TCB::running == nullptr) {
                    regs[REG_A0] = (uint64)-1;
                    break;
                }

                TCB::running->setBlocked(true);
                Scheduler::putSleeping(TCB::running, time);
                TCB::timeSliceCounter = 0;
                TCB::dispatch();

                regs[REG_A0] = (uint64)0;
                break;
            }
            case GETC: {
                regs[REG_A0] = KernelConsole::getInstance()->getc();
                //__asm__ volatile("mv a0, %0" : : "r"(c));
                break;
            }
            case PUTC: {
                char c = (char)regs[REG_A1];
                //__asm__ volatile("mv %0, a1" : "=r"(c));
                KernelConsole::getInstance()->putc(c);
                regs[REG_A0] = (uint64)0;
                break;
            }
        }
    }
    else if(scause == 0x8000000000000001UL){
        //Interrupt: yes, cause code: supervisor software interrupt (timer)
        RiscV::mc_sip(RiscV::SIP_SSIP);

        Scheduler::tick();
        Scheduler::awake();

        TCB::timeSliceCounter++;
        if (TCB::running == nullptr) return;

        if (TCB::timeSliceCounter >= TCB::running->getTimeSlice()) {
            TCB::timeSliceCounter = 0;
            TCB::dispatch();
        }
    }
    else if (scause == 0x8000000000000009UL) {
        //Interrupt: yes, cause code: supervisor external interrupt (console)
        int irq = plic_claim();
        if (irq == CONSOLE_IRQ) {
            KernelConsole::getInstance()->ConsoleInterrupt();
        }

        plic_complete(irq);
    }
    else {
        //Unexpected trap cause, print status registers
        KernelPrintString("ERROR:\n");
        //print(scause)
        KernelPrintString("scause: ");
        KernelPrintInteger(scause);

        //print(sepc)
        KernelPrintString("\nsepc: ");
        KernelPrintInteger(sepc);

        //print(stval)
        KernelPrintString("\nsstatus: ");
        KernelPrintInteger(sstatus);

        //print(stval)
        KernelPrintString("\nstval: ");
        KernelPrintInteger(r_stval());

        KernelConsole::getInstance()->clearBufferIllegalError();

        while (true){}
    }

    w_sepc(sepc);
    w_sstatus(sstatus);
}
