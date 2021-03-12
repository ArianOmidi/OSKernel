#ifndef PCB_H
#define PCB_H

/*
PCB has 3 fields
PC : Points to the the current line reached for that program
start: Points to first line of that program
end: Points to last line of that program
*/
typedef struct PCB {
  int PID;
  int PC;
  int PC_page;
  int PC_offset;
  int pages_max;
  int pageTable[10];
} PCB;

/*
Passes 2 parameters (start , end)
This method creates a PCB with fields set as this:
PC = start
start = start
end = end
*/
PCB* makePCB(int PID, int lastInstruction);

void printPCB(PCB* pcb);

#endif