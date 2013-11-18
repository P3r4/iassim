#ifndef mem_h
#define mem_h

void printfMEM(int, int, int);

void loadMEM(FILE *, int);

uint64_t readMEM(int pos);

void writeMEM(int pos, uint64_t);

void writeLMEM(int pos, uint64_t);

void writeRMEM(int pos, uint64_t);

#endif
