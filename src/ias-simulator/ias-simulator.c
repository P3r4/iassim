#include "headers/base.h"
#include "headers/mem.h"
#include "headers/regs-flags.h"

void main(int argc, char *argv[]){
    int memSize = 150;
    FILE * hex = fopen("../../hex/mxn.hex","r");
    loadMEM(hex, memSize);
    fclose(hex);
    while(isOFF(END_FLAG)){ 
        instCycle();
    }
    printfREGS();
    printfMEM(memSize,76,92);
}

