#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include "headers/mem.h"

void printfMEM(uint64_t *mem, int max, int b, int e){
    int i;
    int mo = 0;
    int mf = max; 
    
    if(e<max){
        mf = e;
    }

    if(b>=0){
        mo = b;
    }
    
    for(i = mo; i<=mf; i++){ 
        printf("%010" PRIx64 "\n", mem[i]);   
    }    
}

void printfREGS(uint64_t *regs){
    char labels[7][5] = {" AC","IBR"," IR","MAR","MBR"," MQ"," PC"};
    int i;    
    for(i = 0; i<7;i++){
        printf("%s: %010" PRIx64 "\n",labels[i], regs[i]);
    }    
}

void main(int argc, char *argv[]){
    uint64_t mem[100];
    uint64_t regs[7] = {0,0,0,0,0,0,0};
    int flags[2];
    
    int memSize = 100;
    FILE * hex = fopen("../../hex/teste.hex","r");
    loadMEM(hex, mem);
    fclose(hex); 
    fetch(mem, regs, flags);
    printfREGS(regs);
    decode(mem, regs, flags);
    printfREGS(regs);
}

