#include "headers/base.h"

uint64_t *mem;

void initMEM(int size){
    mem = malloc(sizeof(uint64_t)*size);
}

uint64_t readMEM(int pos){
    return mem[pos];
}

void writeMEM(int pos, uint64_t bus, uint64_t mask){
    mem[pos] = (mask & mem[pos]) | bus;
}




