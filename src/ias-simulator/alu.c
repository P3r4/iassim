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

void alu(int op, uint64_t ac, uint64_t mbr, uint64_t mq){
    
    if(op == MBR_AC){
        setReg(AC, mbr);
    }else if(op == AC_MBR){
        setReg(MBR, ac);
    }else if(op == ADD){
        setReg(AC, add(ac,mbr));
    }else if(op == SUB){
        setReg(AC, sub(ac,mbr));
    }else if(op == MQ_AC){
        setReg(AC, mq);
    }else if(op == MBR_MQ){
        setReg(MQ, mbr);
    }
    

}
