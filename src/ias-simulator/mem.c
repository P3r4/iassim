#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

#define LCLEAN 0xff000fffff
#define RCLEAN 0xfffffff000

void loadMEM(FILE *hexFile, uint64_t *mem){
    char buffer[12];
    int i = 0;
    while(fgets(buffer,12,hexFile) != NULL){
        mem[i] = strtoull(buffer, NULL, 16);
        i++;
    }
}

void readMEM(uint64_t *mem, int pos, uint64_t *mbr){
    *mbr = mem[pos];
}

void writeMEM(uint64_t *mem, int pos, uint64_t *mbr){
    mem[pos] = *mbr;
}

void writeLMEM(uint64_t *mem, int pos, uint64_t *mbr){
    uint64_t shMbr = *mbr << 20;
    mem[pos] = LCLEAN & mem[pos];
    mem[pos] = shMbr | mem[pos];
}

void writeRMEM(uint64_t *mem, int pos, uint64_t *mbr){
    mem[pos] = RCLEAN & mem[pos];    
    mem[pos] = *mbr | mem[pos];
}




