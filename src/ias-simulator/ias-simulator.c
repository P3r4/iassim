#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include "headers/mem.h"

void main(int argc, char *argv[]){
    int memSize = 100;

    FILE * hex = fopen("../../hex/teste.hex","r");
    loadMEM(hex, memSize);
    fclose(hex); 
    instCycle();
    printfREGS();
}

