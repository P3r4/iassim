#ifndef regs_flags_h
#define regs_flags_h

#define AC 0
#define IBR 1
#define IR 2
#define MAR 3
#define MBR 4
#define MQ 5
#define PC 6
#define MSK 7

#define FETCH_FLAG 0
#define JMPR_FLAG 1
#define END_FLAG 2
#define READMEM_FLAG 3
#define WRITEMEM_FLAG 4

void pcAdd1();

void turnON(int);

void turnOFF(int);

int isON(int);

int isOFF(int);

uint64_t getReg(int);

void setReg(int, uint64_t);

#endif
