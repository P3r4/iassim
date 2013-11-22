#include "headers/base.h"
#include "headers/ctrl-uni.h"
#include "headers/utilities.h"
#include "headers/regs-flags.h"

void cpu(int x, int max, int ma, int mb){
    char c;        
    int ir;
    printfDelim();        
    printf(" Press <s> + <Enter> to show Memory and/or Registers and continue.\n Press <ENTER> to just continue. \n");
    printfDelim();    
    while(isOFF(END_FLAG)){         
        ir = instCycle();
        instType(ir);
        if(x == 1){
            printfPC();
            c  = getchar();            
            if(c == 's'){             
                while ( getchar() != '\n' );                
                printfREGS();
            }else if( c != '\n'){
                while ( getchar() != '\n' );
                printf("\n");
            }
            printfDelim();
        }else if(x == 2){
            printfPC();
            c  = getchar();            
            if(c == 's'){             
                while ( getchar() != '\n' );                
                printfMEM(max,ma,mb);
            }else if( c != '\n'){
                while ( getchar() != '\n' );
                printf("\n");
            }
            printfDelim();            
        }else if(x == 3){
            printfPC();
            c  = getchar();            
            if(c == 's'){             
                while ( getchar() != '\n' );                
                printfREGS();
            }else if( c != '\n'){
                while ( getchar() != '\n' );
                printf("\n");
            }
            printfDelim();
        }
        
    }
    
    printfInstTypeQtts();

}
