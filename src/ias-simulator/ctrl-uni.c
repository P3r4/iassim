#include "headers/base.h"
#include "headers/mem.h"
#include "headers/regs-flags.h"
#include "headers/utilities.h"
#include "headers/alu.h"

/*
char  instructions[INST_QTT][INST_LEN] = {
 "load-mq", "load+mq", "store", "load", "load-neg", "load-abs", "load-abs-neg",
 "jump-l", "jump-r", "jump+l", "jump+r",
 "add", "add-abs", "sub", "sub-abs", "mult", "div", "shift-l", "shift-r",
 "store-l", "store-r", "exit"};

int64_t opcodes[INST_QTT] = {
 10, 9, 33, 1, 2, 3, 4,
 13, 14, 15, 16,
 5, 7, 6, 8, 11, 12, 20, 21,
 18, 19, 0
};
*/

void fetch(){
    uint64_t bus;    
    setReg(MAR,getReg(PC));
    bus = readMEM(getReg(MAR));
    setReg(MBR, bus);
}

void decodeL(){
    uint64_t mbr = getReg(MBR);
    setReg(IBR, getINSTR(mbr));
    setReg(IR, getOPL(mbr));
    setReg(MAR, getADL(mbr));
}

void decodeR(int r){
    uint64_t reg = getReg(r);
    setReg(IR,getOPR(reg));
    setReg(MAR,getADR(reg));
    setReg(PC,getReg(PC)+1);     
}

void fetchCycle(){
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
}

void dealControlSigns(uint64_t ir){
    if(ir == 0){
        turnON(END_FLAG);
    }else if ((ir != 10)&&(ir>=1)&&(ir<=12)){
        turnON(READMEM_FLAG);
    }else if ((ir == 33)||(ir == 18)||(ir == 19)){
        turnON(WRITEMEM_FLAG);
    }
}

void execute(uint64_t ir){
    int op;
    if(ir == 1){
        op = MBR_AC;      
    }else if(ir == 33){
        op = AC_MBR;
    }else if(ir == 5){
        op = ADD;
    }else if(ir == 6){
        op = SUB;
    }else if(ir == 10){
        op = MQ_AC;
    }else if(ir == 9){
        op = MBR_MQ;
    }else if(ir == 11){
        
    }else if(ir == 12){
    }else if(ir == 20){
    }else if(ir == 21){
    }
    alu(op,getReg(AC),getReg(MBR),getReg(MQ));
}

void instCycle(){

    fetchCycle();  
    dealControlSigns(getReg(IR));
    if(isON(READMEM_FLAG)){  
        setReg(MBR,readMEM(getReg(MAR)));
        turnOFF(READMEM_FLAG);
    }        
    execute(getReg(IR));
    if(isON(WRITEMEM_FLAG)){
        writeMEM(getReg(MAR),getReg(MBR));
        turnOFF(WRITEMEM_FLAG);
    }   
} 


