#include "headers/base.h"

uint64_t *mem;

void printfMEM(int max, int b, int e){
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
        printf("%04d: %010" PRIx64 "\n", i, mem[i]);   
    }    
}

void loadMEM(FILE *hexFile, int size){
    char buffer[15];
    int i = 0;
    mem = malloc(sizeof(uint64_t)*size);
    while(fgets(buffer,15,hexFile) != NULL){
        mem[i] = strtoull(buffer, NULL, 16);
        i++;
    }
}

uint64_t readMEM(int pos){
    return mem[pos];
}

void writeMEM(int pos, uint64_t bus, uint64_t mask){
    mem[pos] = (mask & mem[pos]) | bus;
}




