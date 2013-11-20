#include "headers/base.h"
#include "headers/mem.h"
#include "headers/regs-flags.h"

void main(int argc, char *argv[]){
    int memSize = 100;
    FILE * hex = fopen("../../hex/teste_addsub.hex","r");
    loadMEM(hex, memSize);
    fclose(hex);
    while(isOFF(END_FLAG)){ 
        instCycle();
    }
    printfREGS();
    printfMEM(memSize,0,12);
}

