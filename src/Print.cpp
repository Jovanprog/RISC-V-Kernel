#include "../h/Print.hpp"

#include "../h/KernelConsole.hpp"
#include "../lib/hw.h"


void KernelPrintString(char const *string){
    if (string == nullptr) return;

    while(*string != '\0'){
        //Ovo treba da implementiras u syscall_c
        KernelConsole::getInstance()->putc(*string);
        string++;
    }
}

//Vidi da li treba da ti bude heksadecimalan i samim tim pozitivan broj
void KernelPrintInteger(uint64 integer){
    static char digits[] = "0123456789";
    char buf[32];
    int i = 0;
    uint64 x;

    if(integer < 0){
        KernelConsole::getInstance()->putc('-');

        x = (uint64)(-(integer +1)) + 1;
    }
    else{
        x = (uint64)integer;
    }

    i = 0;
    do{
        buf[i++] = digits[x % 10];
    } while((x /= 10) != 0);

    while(--i >= 0){
        KernelConsole::getInstance()->putc(buf[i]);
    }
}