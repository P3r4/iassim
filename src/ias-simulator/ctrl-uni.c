#include "headers/base.h"
#include "headers/mem.h"
#include "headers/regs-flags.h"
#include "headers/utilities.h"
#include "headers/alu.h"

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

int decodeOp(uint64_t ir){
    int op = -1;
    if(ir == 1){
        op = MBR_AC;      
    }else if(ir == 2){
        op = N_MBR_AC;      
    }else if(ir == 3){
        op = A_MBR_AC;      
    }else if(ir == 4){
        op = AN_MBR_AC;      
    }else if(ir == 33){
        op = AC_MBR;
    }else if(ir == 18){
        op = AC_MBR_L;
    }else if(ir == 19){
        op = AC_MBR_R;
    }else if(ir == 5){
        op = ADD;
    }else if(ir == 6){
        op = SUB;
    }else if(ir == 7){
        op = A_ADD;
    }else if(ir == 8){
        op = A_SUB;
    }else if(ir == 10){
        op = MQ_AC;
    }else if(ir == 9){
        op = MBR_MQ;
    }else if(ir == 11){
        op = MUL;        
    }else if(ir == 12){
        op = DIV;
    }else if(ir == 20){
        op = LSH;
    }else if(ir == 21){
        op = RSH;
    }else if((ir == 15)||(ir == 16)){
        op = AC_MAG;
    }
    return op;
}

void memoryAcessControl(uint64_t ir){
    if ((ir != 10)&&(ir>=1)&&(ir<=12)){
        turnON(READMEM_FLAG);
    }else if (ir == 33){
        turnON(WRITEMEM_FLAG);
        setReg(MSK, 0);
    }else if (ir == 18){
        turnON(WRITEMEM_FLAG);
        setReg(MSK, LCLEAN);
    }else if (ir == 19){
        turnON(WRITEMEM_FLAG);
        setReg(MSK, RCLEAN);
    }
}

void getOperands(uint64_t mar){
    if(isON(READMEM_FLAG)){  
        setReg(MBR,readMEM(mar));
        turnOFF(READMEM_FLAG);
    }        
}

void execute(uint64_t ir){    
    int op = decodeOp(ir);    
    alu(op,getReg(AC),getReg(MBR),getReg(MQ));
}

void branchControl(uint64_t ir){
    if(ir == 0){
        turnON(END_FLAG);
    }else if ((ir == 13)||(ir == 14)){
        turnON(FETCH_FLAG);
        if(ir == 14){
            turnON(JMPR_FLAG);            
        }
        setReg(PC,getReg(MAR));
    }else if ((ir == 15)||(ir == 16)){
        if(getReg(MBR) == 0){        
            turnON(FETCH_FLAG);
            if(ir == 16){
                turnON(JMPR_FLAG);            
            }
            setReg(PC,getReg(MAR));
        }
    }
}

void saveResults(uint64_t mar, uint64_t mbr, uint64_t mask){
    if(isON(WRITEMEM_FLAG)){
        writeMEM(mar,mbr,mask);
        turnOFF(WRITEMEM_FLAG);
    }   
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

int instCycle(){
    fetchCycle();
    memoryAcessControl(getReg(IR));
    getOperands(getReg(MAR));    
    execute(getReg(IR));
    branchControl(getReg(IR));
    saveResults(getReg(MAR),getReg(MBR),getReg(MSK));
    return getReg(IR);
} 


