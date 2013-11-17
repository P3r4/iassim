#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "headers/mem.h"


#define ON 1
#define OFF 0

#define AC 0
#define IBR 1
#define IR 2
#define MAR 3
#define MBR 4
#define MQ 5
#define PC 6

#define DR_FLAG 0
#define JMPR_FLAG 1
#define END_FLAG 2

#define OPL 0xff00000000
#define OPR 0xff000
#define ADL 0xfff00000
#define ADR 0xfff
#define INSTR 0xfffff


void fetch(uint64_t *mem, uint64_t *regs, int *flags){
    regs[MAR] = regs[PC];
    readMEM(mem,regs[MAR],&regs[MBR]);
}

void decodeR(uint64_t *mem, uint64_t *regs, int *flags){
    if(flags[JMPR_FLAG] == ON){
        regs[IR] = OPR & regs[MBR];
        regs[MAR] = ADR & regs[MBR];
    }else{
        regs[IR] = OPR & regs[IBR];
        regs[MAR] = ADR & regs[IBR];       
    }
    regs[IR] = regs[IR] >> 12;
    regs[PC] = regs[PC] + 1;        
}

void decode(uint64_t *mem, uint64_t *regs, int *flags){
    if(flags[DR_FLAG] == ON){
        decodeR(mem, regs, flags);           
    }else{
        regs[IBR] = INSTR & regs[MBR];
        regs[IR] = OPL & regs[MBR];
        regs[IR] = regs[IR] >> 32;
        regs[MAR] = ADL & regs[MBR];
        regs[MAR] = regs[MAR] >> 20;
    }
}


void execute(uint64_t *mem, uint64_t *regs, int *flags){

}

void instCycle(uint64_t *mem, uint64_t *regs, int *flags){
    while(flags[END_FLAG] == OFF){
        fetch();
        decode();        
        execute();
        decode();
        execute();
    }
} 


