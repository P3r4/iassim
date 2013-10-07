#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <inttypes.h>

//21 instructions of ias + exit = 22 inst 
#define INST_QTT 22
#define INST_LEN 20
#define STR_LEN 50
#define DATA ".data"
#define TEXT ".text"
#define DATA_START 0
#define TEXT_START 400
#define MEM_SIZE 1000 //words

char  instructions[INST_QTT][INST_LEN] = {
 "load-mq", "load+mq", "store", "load", "load-neg", "load-abs", "load-abs-neg",
 "jump-l", "jump-r", "jump+l", "jump+r",
 "add", "add-abs", "sub", "sub-abs", "mult", "div", "shift-l", "shift-r",
 "store-l", "store-r", "exit"};

uint64_t opcodes[INST_QTT] = {
 10, 9, 33, 1, 2, 3, 4,
 13, 14, 15, 16,
 5, 7, 6, 8, 11, 12, 20, 21,
 18, 19, 0
};

FILE *pIASFile, *pHexFile;
int memPos;

int isInt(char *str){
        char ints[10] = "0123456789";
        int i = 0;
        int j, msg;
        int size = strlen(str);
        size = size -1;
        if(str[0]=='-'){
                i = 1;
        }
        do{
            msg = 1;
            for(j = 0; j<10; j++){
                if(str[i]==ints[j]){
                    msg = 0;
                }        
            }             
            i = i + 1;   
        }while(msg==0 && i<size);
        
        return msg;
}


void eraseSpaces(FILE *pFile, char *c){
    do{
        *c = fgetc(pFile);
    }while( (*c == ' ') || (*c == '\n'));
}

void buildString(FILE *pFile, char *strOut, char *c){
    while( (*c != ' ') && (*c != '\n') && (*c != EOF)){
        strcat(strOut,c);
        *c = fgetc(pFile);
    }
}

//this function get the next String, ignoring line breaks and spaces
//the return if it finds EOF -> -1, and EOF for str
//if ok return 0 and str with a string
//take care of \n in windows SO, its 2 chars there
int getNextString(FILE *pFile, char *str){
    str[0] = '\0';
    int out = 0;    
    char *strOut = malloc(sizeof(str));
    strOut[0] = '\0';
    char *c = malloc(sizeof(char));
    c[0] = '\0';
    
    eraseSpaces(pFile, c);
    buildString(pFile, strOut, c);
    strcpy(str, strOut);
        
    if(*c == EOF) out = -1;
    return out;
}

int identifyInst(char *inst){
    int i = INST_QTT -1;
    while((i > -1) && (strcmp(inst, &instructions[i][0]) != 0)){
        i = i - 1;       
    }
    return i;
}

int isNoParamInst(int i){
    return ((i == 0)||(i == 17)||(i == 18)||(i == 21));
}

int getNextNotTagString(char *str){
    return (!getNextString(pIASFile,str) && str[0] != '.');
}

int getNextOp(char *op){
    return getNextNotTagString(op);
}

void fprintfHexText(char *text){
    uint64_t hexWord = atoi(text);  
    fprintf(pHexFile, "%010"PRIx64"\n",hexWord);
}

void fprintfHexWord(uint64_t hexWord){
    fprintf(pHexFile, "%010"PRIx64"\n", hexWord);
}

void fprintfHexChar(char hexChar){
    fprintf(pHexFile, "%010x\n", hexChar);
}

int isChar(char *data){
    return (data[0]== 39)&&(data[2]== 39);
}

int isString(char *data){
    return (data[0] == 34);
}

int isEndOfString(char * str){
    int len = strlen(str);
    return ( str[len-1] == 34);
}

int fprintfHexString(char * data){         
    int i;    
    int len = strlen(data);     
    for(i = 1; i<len; i++){
        fprintfHexChar(data[i]); 
    }
    while(!getNextString(pIASFile,data) && !isEndOfString(data)){
        len = strlen(data);        
        for(i = 0; i<len; i++){
            fprintfHexChar(data[i]); 
        }        
    }
    len = strlen(data);
    len = len - 1;    
    for(i = 0; i<len; i++){
            fprintfHexChar(data[i]); 
    }    
}


int readData(char *next){
    char data[STR_LEN];
    int out, len, i, is = 0;
    while(getNextNotTagString(&data[0])){
        
        if(isChar(&data[0])){
            fprintfHexChar(data[1]);
        }else if(isString(&data[0]) ){
            fprintfHexString(&data[0]);                        
        }else{ 
            fprintfHexText(&data[0]);
        }     
    }
   
    next[0] = '\0';
    strcpy(next, data);
    return 0;
}

void buildInst(char *str, uint64_t *inst, uint64_t *op, int si, int so){
    int i = identifyInst(str);
    if(i>-1){
        *inst = opcodes[i] << si;
        if(isNoParamInst(i)){
            *op = 0;
        }else{ 
            getNextString(pIASFile, str);
            *op = atoi(str) << so;    
        }
    }  
}

void buildLeftInst(char *str, uint64_t *inst, uint64_t *op){
    buildInst(str, inst, op, 32, 20);   
}

void buildRightInst(char *str, uint64_t *inst, uint64_t *op){
    buildInst(str, inst, op, 12, 0);   
}

int readText(char *next){
    int i;
    uint64_t instl, opl, instr, opr, word;
    char inst[STR_LEN];    
    word = 0; 
    int flag = 1;
     
    while(flag && getNextOp(&inst[0])){     
        buildLeftInst(&inst[0], &instl, &opl);
        if(getNextOp(&inst[0])){ 
            buildRightInst(&inst[0], &instr, &opr);
        }else{
            instr = 0;
            opr = 0;
            flag = 0;
        }      

        word = instl | opl | instr| opr;

        fprintfHexWord(word);
    
    } 
    next[0] = '\0';
    strcpy(next, inst);
    return 0;
}

int readTags(char * tag){
    
    if(!strcmp(tag, DATA)){
        printf("dataStart"); 
        readData(tag);    
    }else{
        return -1;
    }

    if(!strcmp(tag, TEXT) ){
        printf("codeStart");  
        readText(tag);
    }else{
        return -1;        
    }
}

int buildHexFile(char *nameHexFile){
    char tag[STR_LEN];
    pHexFile = fopen(nameHexFile, "w"); 
    if(!getNextString(pIASFile,&tag[0])){
        readTags(&tag[0]);
    }
    pclose(pHexFile);
}

int main(int argc, char *argv[]){
    pIASFile = fopen(argv[1],"r");
    if(pIASFile == 0){
        printf("error-msg: IAS File Not Found.\n");    
        return -1;    
    }else{
       buildHexFile("none");     
       pclose(pIASFile);
       return 0;
    }    
}
