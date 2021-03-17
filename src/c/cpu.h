#ifndef CPU_H
#define CPU_H

#define DEFAULT_QUANTA 2

struct CPU {
  int IP;
  char IR[1000];
  int offest;
  int quanta;
} CPU;

int run(int);

#endif
