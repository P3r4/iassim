#include "headers/base.h"
#include "headers/mem.h"
#include "headers/regs-flags.h"

int type[5] = {0,0,0,0,0};

int dataTransfer(int ir){
    return (ir == 10)|| (ir == 9) ||(ir == 33)||((ir>=1)&&(ir<=4));
}

int uncondBranch(int ir){
    return ((ir == 13)|| (ir == 14));
}

int condBranch(int ir){
    return ((ir == 15)|| (ir == 16));
}

int arithmetic(int ir){
    return ((ir>=5)&&(ir<=8))||(ir == 11)||(ir == 12)||(ir == 20)||(ir == 21);
}

int addressModify(int ir){
    return ((ir == 18)|| (ir == 19));
}

void instType(int ir){
    int t = -1;
    if(dataTransfer(ir)){
        t = 0;
    }else if(uncondBranch(ir)){
        t = 1;
    }else if(condBranch(ir)){
        t = 2;
    }else if(arithmetic(ir)){
        t = 3;
    }else if(addressModify(ir)){
        t = 4;
    } 
  
    if(t>-1) type[t] = type[t] +1;

}

uint64_t getOPL(uint64_t word){
    return (word & OPL) >> 32;
}

uint64_t getOPR(uint64_t word){
    return (word & OPR) >> 12;
}

uint64_t getADL(uint64_t word){
    return (word & ADL) >> 20;
}

uint64_t getADR(uint64_t word){
    return (word & ADR);
}

uint64_t getINSTR(uint64_t word){
    return (word & INSTR);
}

int getMAG(uint64_t value){
   return (MAG & value) >> 39; 
}

int64_t getNUM(uint64_t value){
   return (NUM & value); 
}

int64_t toComp2(uint64_t word){
    int64_t num = getNUM(word);
    if(getMAG(word) == 1){
        num = -num;
    }
    //printf("c2: %010" PRId64 "\n", num);
    return num;    
}

uint64_t toMag(int64_t num){
    uint64_t word;
    if(num<0){
        word = -num;
        word = getNUM(word) | MAG;
    }else{
        word = getNUM(num);
    }
    return word;    
}

void printfDelim(){
    printf("------------------------------------------------------------\n");
}

int printfInstTypeQtts(){
    char labels[6][30] = {"Data Transfer", "Uncondicional Branch","Condicional Branch","Arithmetic","Address Modify"};
    int i;
    printf(" Quantity of Executed Instructions \n");    
    for(i = 0; i<5; i++){
        printf("        %s: %i\n",labels[i],type[i]);
    }
    printfDelim();
}

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
    printf(" Memory(%i:%i)\n",mo,mf);    
    for(i = mo; i<=mf; i++){ 
        printf("        %04d: %010" PRIx64 "\n", i, readMEM(i));   
    }
    printfDelim();
}

void printfPC(){
   printf(" PC: %010" PRIx64 "\n",getReg(PC) );    
}

char* printfONOFF(int on){
    if(on){
        return "ON";
    }else{
        return "OFF";
    }
}

void printfREGS(){
    int i;
    char labels[8][5] = {"AC","IBR","IR","MAR","MBR","MQ","PC", "MSK"};    
    printf(" Registers\n");
    for(i = 0; i<8;i++){
        printf("        %s: %010" PRIx64 "\n", labels[i], getReg(i));
    }
    
    char flabels[5][15] = {"FETCH_FLAG", "JMPR_FLAG", "END_FLAG", "READMEM_FLAG", "WRITEMEM_FLAG"};
    for(i = 0; i<5;i++){
        printf("        %s: %d\n", flabels[i], isON(i));
    }
    printfDelim();    
}

void loadMEM(FILE *hexFile, int size){
    char buffer[15];
    int i = 0;
    initMEM(size);
    while(fgets(buffer,15,hexFile) != NULL){
        writeMEM(i,strtoull(buffer, NULL, 16),0);
        i++;
    }
    printfDelim();
}

printfHelp(){
    printf(" Press <m> + <Enter> for Memory,\n       <q> + <Enter> for Executed Instructions,\n       <r> + <ENTER> for Registers,\n       <h> + <Enter> for HELP.\n");
    printfDelim();    
}

void cpuStatus(int max, int ma, int mb){
    char c;
    printfPC();
    printf(" > ");    
    c  = getchar(); 
    while(c !='\n'){                           
        while ( getchar() != '\n' );                                  
        if(c == 'r'){
            printfREGS();                
        }else if(c == 'm'){
            printfMEM(max,ma,mb);                
        }else if(c == 'q'){
            printfInstTypeQtts();
        }else if( c == 'h'){
            printfHelp();
        }
        printf(" > ");    
        c  = getchar();
    }
    printfDelim();
       
}

