#include "shell.h"
#include "ram.h"
#include "pcb.h"
#include "cpu.h"

#include <stdio.h>
#include <stdlib.h>

// --- READY LIST FUNCTIONS --- //

typedef struct Node{
	PCB* pcb;
	struct Node* next;
} Node;

typedef struct ReadyList{
	Node* head;
	Node* tail;
	int n;
} ReadyList;

ReadyList *readyList;

ReadyList* initReadyList(){
	ReadyList* rl = (ReadyList*) malloc(sizeof(ReadyList));
	rl->head = NULL;
	rl->tail = NULL;
	rl->n = 0;

	return rl;
}

int addToReady(PCB* pcb){
	Node* tmp = (Node*) malloc(sizeof(Node));
	
	if (tmp == NULL){
		printf("Couldn't allocate memory for ReadyList Node\n");
		return 1;
	}

	tmp->pcb = pcb;
	tmp->next = NULL;

	if (readyList->n == 0){
	       readyList->head = tmp;
	} else {
		readyList->tail->next = tmp;	
	}
	readyList->tail = tmp;

	readyList->n++;
	
	return 0;
}

void placeHeadAtTail(){
	if (readyList->n > 1){
		Node* tmp = readyList->head;
		readyList->tail->next = tmp;
		readyList->head = tmp->next;
		readyList->tail = tmp;
		tmp->next = NULL; 
	}
}

void removePCB(){
	if (readyList->n == 0)
		return;

	PCB* pcb = readyList->head->pcb;
	Node* tmp = readyList->head;

	clearRAM(pcb->start, pcb->end);

	readyList->head = tmp->next;
	if (readyList->n == 1) {
		readyList->head = NULL;
		readyList->tail = NULL;
	}
	readyList->n = readyList->n - 1;

	free(tmp);
	free(pcb);
}

void printReadyList(){
	Node* node = readyList->head;

	printf("\nReady List: %d PCBS\n", readyList->n);

	if (readyList->n > 0){
	printf("\tHead PCB: PC = %d, start = %d, end = %d\n", readyList->head->pcb->PC, readyList->head->pcb->start, readyList->head->pcb->end);
	printf("\tTail PCB: PC = %d, start = %d, end = %d\n\n", readyList->tail->pcb->PC, readyList->tail->pcb->start, readyList->tail->pcb->end);

	while(node != NULL){
		printf("\tPCB: PC = %d, start = %d, end = %d\n", node->pcb->PC, node->pcb->start, node->pcb->end);
		node = node->next;
	}

	printf("\n");
	}
}


// --- KERNEL FUNCTIONS --- //

int myinit(char *filename){
        FILE *program = fopen(filename, "r");
        if (program == NULL){
                printf("Error: Script %s does not exist\n", filename);
                return 1;
        }

	int start, end;

	// Add File to RAM
	addToRAM(program, &start, &end);
	fclose(program);

	// Make PCB
	PCB* pcb = makePCB(start, end);
	if (pcb == NULL)
		printf("ERROR: unable to create PCB for program %s\n", filename);

	// Add PCB to Ready List
	addToReady(pcb);
	
        return 0;
}

int scheduler(){
	while (readyList->n > 0){
		setCPU(readyList->head->pcb->PC);
		
		int quanta = 2;
		int errorCode = runCPU(quanta, readyList->head->pcb->end);

		if (errorCode == -1) {
			removePCB();
		} else {
			readyList->head->pcb->PC += quanta ;
			placeHeadAtTail();
		}
	}	

	return 0;
}

int main(int argc, char* argv[]){
	// Init RAM, CPU & Ready List
	emptyRAM();
	initCPU();
	readyList = initReadyList();

	printf("Kernel 1.0 loaded!\n");
	shellUI();
}


