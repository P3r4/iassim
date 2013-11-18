#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

#define LCLEAN 0xff000fffff
#define RCLEAN 0xfffffff000

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
        printf("%010" PRIx64 "\n", mem[i]);   
    }    
}

void loadMEM(FILE *hexFile, int size){
    char buffer[12];
    int i = 0;
    mem = malloc(sizeof(uint64_t)*size);
    while(fgets(buffer,12,hexFile) != NULL){
        mem[i] = strtoull(buffer, NULL, 16);
        i++;
    }
}

uint64_t readMEM(int pos){
    return mem[pos];
}

void writeMEM(int pos, uint64_t bus){
    mem[pos] = bus;
}

void writeLMEM(int pos, uint64_t bus){
    uint64_t sh20Bus = bus << 20;
    mem[pos] = LCLEAN & mem[pos];
    mem[pos] = sh20Bus | mem[pos];
}

void writeRMEM(int pos, uint64_t bus){
    mem[pos] = RCLEAN & mem[pos];    
    mem[pos] = bus | mem[pos];
}

