#ifndef RAM_H
#define RAM_H

void emptyRAM();

void clearRAM(int start, int end);
void addToRAM(FILE *p, int *start, int *end);

#endif