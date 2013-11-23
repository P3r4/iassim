#ifndef utils_h
#define utils_h
void printfMEM(int, int, int);

void printfREGS();

void printfInstTypeQtts();

void printfDelim();

void printfPC();

void printfHelp();

void cpuStatus(int , int , int );

void loadMEM(FILE *, int);

uint64_t getOPL(uint64_t);

uint64_t getOPR(uint64_t);

uint64_t getADL(uint64_t);

uint64_t getADR(uint64_t);

uint64_t getINSTR(uint64_t);

int64_t toComp2(uint64_t);

uint64_t toMag(int64_t);

#endif
