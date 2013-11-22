#include "headers/base.h"

#define ON 1
#define OFF 0

uint64_t regs[8] = {0,0,0,0,0,0,0,0};

int flags[5] = {1,0,0,0,0};

void pcAdd1(){
    flags[6] = flags[6]+1;
}

void turnON(int flag){
    flags[flag] = ON;
}

void turnOFF(int flag){
    flags[flag] = OFF;
}

int isON(int flag){
    return (flags[flag] == ON);
}

int isOFF(int flag){

    return (flags[flag] == OFF);
}

uint64_t getReg(int reg){
    return regs[reg];
}

void setReg(int reg, uint64_t value){
    regs[reg] = value;
}
