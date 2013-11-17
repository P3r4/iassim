#ifndef mem_h
#define mem_h

void loadMEM(FILE *, uint64_t *);

void readMEM(uint64_t *, int pos, uint64_t *);

void writeMEM(uint64_t *, int pos, uint64_t *);

void writeLMEM(uint64_t *, int pos, uint64_t *);

void writeRMEM(uint64_t *, int pos, uint64_t *);

#endif
