#ifndef PROJECT_BASE_V1_1_RISCV_HPP
#define PROJECT_BASE_V1_1_RISCV_HPP

#include "../lib/hw.h"

class RiscV {
public:
    //Pop sstatus.spp i sstatus.spie bitove (mora da bude ne inline funkcija)
    static void popSppSpie();
    //Procitaj registar scause
    static uint64 r_scause();
    //Upisi u registar scause
    static void w_scause(uint64 scause);

    //Procitaj registar sepc
    static uint64 r_sepc();
    //Upisi u registar sepc
    static void w_sepc(uint64 sepc);

    //Procitaj registar stvec
    static uint64 r_stvec();
    //Upisi u registar stvec
    static void w_stvec(uint64 stvec);

    //Procitaj registar stval
    static uint64 r_stval();
    //Upisi u registar stval
    static void w_stval(uint64 stval);

    //Procitaj registar sip
    static uint64 r_sip();
    //Upisi u registar sip
    static void w_sip(uint64 sip);

    enum BitMaskSip{
        SIP_SSIP = (1 << 1),
        SIP_STIP = (1 << 5),
        SIP_SEIP = (1 << 9)
    };

    //Mask set registar sip
    static void ms_sip(uint64 mask);
    //Mask clear registar sip
    static void mc_sip(uint64 mask);

    enum BitMaskSstatus{
        SSTATUS_SIE = (1 << 1),
        SSTATUS_SPIE = (1 << 5),
        SSTATUS_SPP = (1 << 8)
    };

    //Mask set registar sstatus
    static void ms_sstatus(uint64 mask);
    //Mask clear registar sstatus
    static void mc_sstatus(uint64 mask);

    //Procitaj registar sstatus
    static uint64 r_sstatus();
    //Upisi u registar sstatus
    static void w_sstatus(uint64 sstatus);

    //Supervisor trap
    static void supervisorTrap();

private:
    //Supervisor trap handler
    static void handleSupervisorTrap(uint64* regs);
};

inline uint64 RiscV::r_scause(){
    uint64 volatile scause;
    __asm__ volatile ("csrr %[scause], scause" : [scause] "=r"(scause));
    return scause;
}

inline void RiscV::w_scause(uint64 scause) {
    __asm__ volatile ("csrw scause, %[scause]" : : [scause] "r"(scause));
}

inline uint64 RiscV::r_sepc(){
    uint64 volatile sepc;
    __asm__ volatile ("csrr %[sepc], sepc" : [sepc] "=r"(sepc));
    return sepc;
}

inline void RiscV::w_sepc(uint64 sepc) {
    __asm__ volatile ("csrw sepc, %[sepc]" : : [sepc] "r"(sepc));
}

inline uint64 RiscV::r_stvec(){
    uint64 volatile stvec;
    __asm__ volatile ("csrr %[stvec], stvec" : [stvec] "=r"(stvec));
    return stvec;
}

inline void RiscV::w_stvec(uint64 stvec) {
    __asm__ volatile ("csrw stvec, %[stvec]" : : [stvec] "r"(stvec));
}

inline uint64 RiscV::r_stval(){
    uint64 volatile stval;
    __asm__ volatile ("csrr %[stval], stval" : [stval] "=r"(stval));
    return stval;
}

inline void RiscV::w_stval(uint64 stval) {
    __asm__ volatile ("csrw stval, %[stval]" : : [stval] "r"(stval));
}

inline uint64 RiscV::r_sip(){
    uint64 volatile sip;
    __asm__ volatile ("csrr %[sip], sip" : [sip] "=r"(sip));
    return sip;
}

inline void RiscV::w_sip(uint64 sip) {
    __asm__ volatile ("csrw sip, %[sip]" : : [sip] "r"(sip));
}

inline void RiscV::ms_sip(uint64 mask){
    __asm__ volatile ("csrs sip, %[mask]" : : [mask] "r"(mask));
}

inline void RiscV::mc_sip(uint64 mask){
    __asm__ volatile ("csrc sip, %[mask]" : : [mask] "r"(mask));
}

inline uint64 RiscV::r_sstatus(){
    uint64 volatile sstatus;
    __asm__ volatile ("csrr %[sstatus], sstatus" : [sstatus] "=r"(sstatus));
    return sstatus;
}

inline void RiscV::w_sstatus(uint64 sstatus) {
    __asm__ volatile ("csrw sstatus, %[sstatus]" : : [sstatus] "r"(sstatus));
}

inline void RiscV::ms_sstatus(uint64 mask){
    __asm__ volatile ("csrs sstatus, %[mask]" : : [mask] "r"(mask));
}

inline void RiscV::mc_sstatus(uint64 mask){
    __asm__ volatile ("csrc sstatus, %[mask]" : : [mask] "r"(mask));
}


#endif //PROJECT_BASE_V1_1_RISCV_HPP
