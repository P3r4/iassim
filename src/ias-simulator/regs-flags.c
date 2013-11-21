#include "headers/base.h"

#define ON 1
#define OFF 0

char labels[8][5] = {" AC","IBR"," IR","MAR","MBR"," MQ"," PC", "MSK"};

uint64_t regs[8] = {0,0,0,0,0,0,0,0};

int flags[5] = {1,0,0,0,0};

void printfREGS(){
    int i;    
    for(i = 0; i<7;i++){
        printf("%s: %010" PRIx64 "\n",labels[i], regs[i]);
    }    
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
