#include "headers/base.h"
#include "headers/regs-flags.h"
#include "headers/utilities.h"

uint64_t add(uint64_t ac, uint64_t mbr){
    int64_t out = toComp2(ac) + toComp2(mbr);
    return toMag(out);     
}

uint64_t sub(uint64_t ac, uint64_t mbr){
    int64_t out = toComp2(ac) - toComp2(mbr);
    return toMag(out);     
}

int64_t prod(uint64_t mbr, uint64_t mq){
    int64_t num = toComp2(mbr) * toComp2(mq);
    return num;   
}

uint64_t msb(int64_t num){
    int64_t s;
    if(num<0){
        s = (-num) >> 39;
        s = -s;    
    }else{
        s = num >> 39;
    }    
    return toMag(s);
}

uint64_t lsb(int64_t out){
    return toMag(out);
}

uint64_t quoc(uint64_t ac, uint64_t mbr){
    int64_t out = toComp2(ac) / toComp2(mbr);
    return toMag(out);   
}

uint64_t mod(uint64_t ac, uint64_t mbr){
    int64_t out = toComp2(ac) % toComp2(mbr);
    return toMag(out);   
}

uint64_t lsh(uint64_t ac){
    uint64_t out = 0; 
    if(getMAG(ac)==1){
        out = getNUM(ac) << 1;
        out = getNUM(out) | MAG;            
    }else{
        out = ac << 1;
        out = getNUM(out);    
    }
    return out;
}

uint64_t rsh(uint64_t ac){
    uint64_t out; 
    if(getMAG(ac)==1){
        out = getNUM(ac) >> 1;
        out = out | MAG;            
    }else{
        out = ac >> 1;    
    }
    return out;
}


void alu(int op, uint64_t ac,  uint64_t mar, uint64_t mbr, uint64_t mq){
    
    if(op == MBR_AC){
        setReg(AC, mbr);
    }else if(op == AC_MBR){
        setReg(MBR, ac);
        setReg(MSK, 0);
    }else if(op == AC_MBR_L){
        setReg(MBR, getADR(ac) << 20);
        setReg(MSK, LCLEAN);
    }else if(op == AC_MBR_R){
        setReg(MBR, getADR(ac));
        setReg(MSK, RCLEAN);
    }else if(op == ADD){        
        setReg(AC, add(ac,mbr));
    }else if(op == SUB){        
        setReg(AC, sub(ac,mbr));
    }else if(op == MQ_AC){
        setReg(AC, mq);
    }else if(op == MBR_MQ){
        setReg(MQ, mbr);
    }else if(op == MUL){
        int64_t out = prod(mbr,mq);        
        setReg(AC,msb(out));
        setReg(MQ,lsb(out));
    }else if(op == DIV){
        setReg(MQ, quoc(ac,mbr));
        setReg(AC, mod(ac,mbr));
    }else if(op == LSH){
        setReg(AC,lsh(ac));       
    }else if(op == RSH){
        setReg(AC,rsh(ac));
    }else if(op == MAR_PC){
        setReg(PC,mar);
    }else if(op == MAR_PC_C){
        if(getMAG(ac) == 0) setReg(PC,mar);
    }
    
}
