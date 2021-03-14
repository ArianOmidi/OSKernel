#include <stdio.h>
#include <stdlib.h>

typedef struct PCB {
  int PID;
  int PC;
  int PC_page;
  int PC_offset;
  int pages_max;
  int pageTable[10];
} PCB;

PCB *makePCB(int PID, int numOfPages) {
  PCB *pcb = (PCB *)malloc(sizeof(PCB));

  pcb->PID = PID;
  pcb->PC = 0;
  pcb->PC_page = 0;
  pcb->PC_offset = 0;
  pcb->pages_max = numOfPages;

  for (int i = 0; i < 10; i++) {
    pcb->pageTable[i] = -1;
  }

  return pcb;
}

// For debugging
void printPCB(PCB *pcb) {
  printf("\n\t---------- PCB ----------\n");
  printf("\tPID: %d, PageCount: %d\n", pcb->PID, pcb->pages_max);
  printf("\tPC_pg: %d, PC_off: %d\n", pcb->PC_page, pcb->PC_offset);
  printf("\t----- Page Table -----\n");
  for (int i = 0; i < 10; i += 2) {
    printf("\t%d -> %d\t%d -> %d\n", i, pcb->pageTable[i], i + 1,
           pcb->pageTable[i + 1]);
  }
  printf("\t----------------------\n");
}