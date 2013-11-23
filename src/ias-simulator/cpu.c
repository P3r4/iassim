#include "headers/base.h"
#include "headers/ctrl-uni.h"
#include "headers/utilities.h"
#include "headers/regs-flags.h"

void cpu(int x, int max, int ma, int mb){
    char c;        
    int ir;
    int line = 0;
    if(x!=0) printfHelp();
    int pc;
    if(x == 2){
        printf("Stop where PC >");
        scanf("%d", &pc);
        while ( getchar() != '\n' );
    }
    while(isOFF(END_FLAG)){         
        if(x == 1){ 
            cpuStatus(max,ma,mb);
        }else if(x == 2){
            
            if(pc == getReg(PC)){
                cpuStatus(max,ma,mb);
                line++;
            }
        }       
        ir = instCycle();
        instType(ir);
        if(line == 2){
            line =0;
            printf(" Stop where PC >");
            scanf("%d", &pc);
            while ( getchar() != '\n' );
        }
    }
    if(x == 0){
        printfInstTypeQtts();        
        printfMEM(max,ma,mb);                
        printfREGS();                
    }
}
