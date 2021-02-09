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
} ReadyList;

ReadyList *readyList;

ReadyList* initReadyList(){
	ReadyList* rl = (ReadyList*) malloc(sizeof(ReadyList));
	rl->head = NULL;
	rl->tail = NULL;

	return rl;
}

int addPCBToReadyList(PCB* pcb){
	Node* tmp = (Node*) malloc(sizeof(Node));
	
	if (tmp == NULL){
		printf("Couldn't allocate memory for ReadyList Node\n");
		return 1;
	}

	tmp->pcb = pcb;
	tmp->next = NULL;

	if(readyList->head == NULL)
	       readyList->head = tmp;
	if (readyList->tail != NULL)
		readyList->tail->next = tmp;	
	readyList->tail = tmp;

	return 0;
}

void placeHeadAtTail(){
	if (readyList->tail != readyList->head){
		readyList->tail->next = readyList->head;
		readyList->head = readyList->head->next;
		readyList->tail = readyList->head;
		readyList->tail->next = NULL; 
	}
}


int myinit(char *filename){
        FILE *program = fopen(filename, "r");
        if (program == NULL){
                printf("Error: Script %s does not exist\n", filename);
                return 1;
        }

        // TODO
        return 0;
}

int main(int argc, char* argv[]){
	// Init RAM
	emptyRAM();

	printf("Kernel 1.0 loaded!\n");
	shellUI();
}


