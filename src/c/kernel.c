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

void initReadyList(){
	ReadyList* rl = (ReadyList*) malloc(sizeof(ReadyList));
	rl->head = NULL;
	rl->tail = NULL;
	rl->n = 0;

	readyList = rl;
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

void emptyReadyList(){
        while (readyList->head != NULL){
                removePCB();
        }
}


// --- KERNEL FUNCTIONS --- //

int myinit(char *filename){
        FILE *program = fopen(filename, "r");
        if (program == NULL){
                printf("Error: Script %s does not exist\n", filename);
		emptyReadyList();
                return 1;
        }

	int start, end;

	// Add File to RAM
	addToRAM(program, &start, &end);
	fclose(program);
	if (end < 0) {
		emptyReadyList();
		return 1;
	}

	// Make PCB
	PCB* pcb = makePCB(start, end);
	if (pcb == NULL){
		printf("ERROR: unable to create PCB for program %s\n", filename);
 		emptyReadyList();
                return 1;
	}

	// Add PCB to Ready List
	addToReady(pcb);
	
        return 0;
}

int scheduler(){
	int errorCode;
	int quanta = getQuanta();
	
	while (readyList->n > 0){
		// Set CPU IP to the PC	
		setCPU(readyList->head->pcb->PC);
		
		// If the program executes in under a quanta, give it the amount of CPU time it needs
		// to complete the program then remove the PCB from the Ready List
		// Else compute quanta lines and add PCB to tail of Ready List
		if (quanta > readyList->head->pcb->end - readyList->head->pcb->PC){
		 	errorCode = run(readyList->head->pcb->end - readyList->head->pcb->PC + 1);
			removePCB();
		} else {
			errorCode = run(quanta);
			readyList->head->pcb->PC += quanta ;
                        placeHeadAtTail();
		}
	}
	
	emptyRAM();
	return 0;
}

int main(int argc, char* argv[]){
	// Init RAM, CPU & Ready List
	emptyRAM();
	initCPU();
	initReadyList();

	printf("Kernel 1.0 loaded!\n");
	shellUI();
}


