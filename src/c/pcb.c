#include <stdio.h>
#include <stdlib.h>

typedef struct PCB{
	int PC;
	int start;
	int end;
} PCB;


PCB* makePCB(int start, int end){
	PCB *pcb = (PCB *) malloc(sizeof(PCB));

	if (pcb != NULL){
		pcb->PC = start;
		pcb->start = start; 
		pcb->end = end;
	}

	return pcb;
}
