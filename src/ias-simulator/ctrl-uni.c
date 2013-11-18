#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "headers/mem.h"
#include "headers/regs-flags.h"

#define OPL 0xff00000000
#define OPR 0xff000
#define ADL 0xfff00000
#define ADR 0xfff
#define INSTR 0xfffff

uint64_t getOPL(uint64_t word){
    return (word & OPL) >> 32;
}

uint64_t getOPR(uint64_t word){
    return (word & OPR) >> 12;
}

uint64_t getADL(uint64_t word){
    return (word & ADL) >> 20;
}

uint64_t getADR(uint64_t word){
    return (word & OPL);
}

uint64_t getINSTR(uint64_t word){
    return (word & INSTR);
}

void fetch(){
    uint64_t bus;    
    setReg(MAR,getReg(PC));
    bus = readMEM(getReg(MAR));
    setReg(MBR, bus);
}

void decodeL(){
    
    setReg(IBR, getINSTR(getReg(MBR)));
    setReg(IR, getOPL(getReg(MBR)));
    setReg(MAR, getADL(getReg(MBR)));
    
}

void decodeR(int reg){
    setReg(IR,getOPR(getReg(reg)));
    setReg(MAR,getADR(getReg(reg)));
    setReg(PC,getReg(PC)+1);     
}

void updateFlags(){
    if((getReg(IR) != 10)&&(getReg(IR)>=1)&&(getReg(IR)<=12)){
        turnON(READMEM_FLAG);
    }
}

void execute(){
        
    if(getReg(IR) == 0){
        turnON(END_FLAG);
    }else if(getReg(IR) == 10){
        
    }else if(getReg(IR) == 9){
        
    }else if(getReg(IR) == 33){
        //traz AC para MBR
        writeMEM(getReg(MAR), getReg(MBR));    
    }else if(getReg(IR) == 1){
        
    }else if(getReg(IR) == 2){
        
    }else if(getReg(IR) == 3){
        
    }else if(getReg(IR) == 4){
        
    }else if(getReg(IR) == 13){
    }else if(getReg(IR) == 14){
    }else if(getReg(IR) == 15){
    }else if(getReg(IR) == 16){
    }else if(getReg(IR) == 5){
        
    }else if(getReg(IR) == 7){
        
    }else if(getReg(IR) == 6){
        
    }else if(getReg(IR) == 8){
        
    }else if(getReg(IR) == 11){
        
    }else if(getReg(IR) == 12){
        
    }else if(getReg(IR) == 20){
    }else if(getReg(IR) == 21){
    }else if(getReg(IR) == 18){
        //traz AC para MBR
        writeLMEM(getReg(MAR), getReg(MBR));
    }else if(getReg(IR) == 19){
        //traz AC para MBR
        writeRMEM(getReg(MAR), getReg(MBR));
    }
}


void instCycle(){
    while(isOFF(END_FLAG)){        
        if(isON(FETCH_FLAG)){
            fetch();
            if(isOFF(JMPR_FLAG)){
                decodeL();
                turnOFF(FETCH_FLAG);
            }else{
                decodeR(MBR);
                turnOFF(JMPR_FLAG);
            }       

        }else{
            decodeR(IBR);
            turnON(FETCH_FLAG);        
        }       

        execute();

    }
} 


