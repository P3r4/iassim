#include "headers/base.h"

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
    return (word & ADR);
}

uint64_t getINSTR(uint64_t word){
    return (word & INSTR);
}

int getMAG(uint64_t value){
   return (MAG & value) >> 39; 
}

int64_t getNUM(uint64_t value){
   return (NUM & value); 
}

int64_t toComp2(uint64_t word){
    int64_t num = getNUM(word);
    if(getMAG(word) == 1){
        num = -num;
    }
    //printf("c2: %010" PRId64 "\n", num);
    return num;    
}

uint64_t toMag(int64_t num){
    uint64_t word;
    if(num<0){
        word = -num;
        word = getNUM(word) | MAG;
    }else{
        word = getNUM(num);
    }
    return word;    
}




