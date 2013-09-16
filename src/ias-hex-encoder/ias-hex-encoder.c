#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <inttypes.h>

//21 instructions of ias + exit = 22 inst 
#define INST_QTT 22
#define INST_LEN 20
#define MEM ".mem"
#define PC ".pc"
#define DATA ".data"
#define CODE ".code"

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

FILE *pFile, *pOutFile;
//1000 words of 5bytes = 5MB (max 20480 bytes = 20 MB)
int memSize = 1000, pcStart = 0, dataStart, codeStart, memPos = 0;

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


int isTagValue(char * tagValue){
    return (tagValue[0] == '.') && (strlen(tagValue)>1) && isInt(tagValue);
}

int isNoParamInst(int i){
    return ((i == 0)||(i == 17)||(i == 18)||(i == 21));
}

int readData(char *next){
    char data[40];
    int out, len, i, is = 0;
    uint64_t outn;
    while( !getNextString(pFile, &data[0]) && data[0] != '.'){
        
        if((data[0]== 39)&&(data[2]== 39)){
          //  printf("%x %x \n", memPos, data[1]);
            fprintf(pOutFile, "%x %x ", memPos, data[1]);
            memPos = memPos + 1; 
        }else if( is || (data[0] == 34)){
            len = strlen(&data[0]);
            if(data[len-1] == 34){
                len = len - 1;
            }else{
                strcat(&data[0]," ");
                len = len + 1;
                is = 1;    
            }            
            for(i = 1; i<len; i++){
           //     printf("%x %x\n", memPos, data[i]);
                fprintf(pOutFile, "%x %x ", memPos, data[i]);
                memPos = memPos + 1; 
            }            
        }else{
            outn = atoi(&data[0]);
           // printf("%x %" PRIx64 "\n", outn);
            fprintf(pOutFile,"%x %" PRIx64 " ", memPos, outn);
            memPos = memPos + 1; 
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
            getNextString(pFile, str);
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

int readCode(char *next){
    int i;
    uint64_t instl, opl, instr, opr, word;
    char inst[40];    
    word = 0; 
    int flag = 1;
     
    while(flag && !getNextString(pFile,&inst[0]) && inst[0] != '.'){     
        buildLeftInst(&inst[0], &instl, &opl);
        if(!getNextString(pFile,&inst[0]) && inst[0] != '.'){ 
            buildRightInst(&inst[0], &instr, &opr);
        }else{
            instr = 0;
            opr = 0;
            flag = 0;
        }      

        word = instl | opl | instr| opr;
        //printf("%" PRIx64 "\n", word);

        fprintf(pOutFile,"%x %" PRIx64 " ", memPos,word);
        memPos = memPos + 1;
    
/*printf("%" PRIx64 "\n", instl);
        printf("%" PRIx64 "\n", opl);
        printf("%" PRIx64 "\n", instr);
        printf("%" PRIx64 "\n", opr);*/
    } 
    next[0] = '\0';
    strcpy(next, inst);
  //  printf(">>>>%s", next);
    return 0;
}




int readTags(char * tag){
    
    if(!strcmp(tag, MEM)){
        getNextString(pFile, tag);
        if( isTagValue(tag) ){
            memSize = atoi(&tag[1]);
            printf("memSize: %i\n", memSize); 
            getNextString(pFile, tag);
            readTags(tag);
        }else return -1;   

    }else if( !strcmp(tag, PC) ){
        getNextString(pFile, tag);
        if(isTagValue(tag)){
            pcStart = atoi(&tag[1]);
            printf("pcStart: %i\n", pcStart);  
            getNextString(pFile, tag);
            readTags(tag);       
        }else return -1;
         
    }else if( !strcmp(tag, DATA) ){
        getNextString(pFile,tag);
        if(isTagValue(tag)){
            dataStart = atoi(&tag[1]);
            memPos = dataStart;
            printf("dataStart: %i\n", dataStart); 
            readData(tag);  
            readTags(tag);  
        }else return -1;
  
    }else if( !strcmp(tag, CODE) ){
        getNextString(pFile,tag);
        if(isTagValue(tag)){
            codeStart = atoi(&tag[1]);
            memPos = codeStart;
            printf("codeStart: %i\n", codeStart);  
            readCode(tag);
            readTags(tag); 
        }else return -1;

    }
}

int buildHexFile(char *nameOutFile){
    char tag[40];
    pOutFile = fopen(nameOutFile, "w"); 
    fprintf(pOutFile,"%s","        ");  
    if(!getNextString(pFile,&tag[0])){
        readTags(&tag[0]);
    }
    fseek ( pOutFile , 0 , SEEK_SET);
    fprintf(pOutFile, "%x %x ", memSize-1, pcStart);
    pclose(pOutFile);
    //printf("tag: %s\n",tag);
}

int main(int argc, char *argv[]){
    pFile = fopen(argv[1],"r");
    if(pFile == 0){
        printf("error-msg: IAS File Not Found.\n");    
        return -1;    
    }else{
       buildHexFile("none");     
       pclose(pFile);
       return 0;
    }    
}
