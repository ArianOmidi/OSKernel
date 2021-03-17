#ifndef KERNEL_H
#define KERNEL_H

#include "pcb.h"

typedef struct ReadyQueueNode {
  PCB* PCB;
  struct ReadyQueueNode* next;
} ReadyQueueNode;

ReadyQueueNode* head;

/*
Adds a pcb to the tail of the linked list
*/
void addToReady(struct PCB*);

/*
Returns the size of the queue
*/
int size();

/*
Pops the pcb at the head of the linked list.
pop will cause an error if linkedlist is empty.
Always check size of queue using size()
*/
PCB* pop();

int scheduler();

/*
Flushes every pcb off the ready queue in the case of a load error
*/
void emptyReadyQueue();

#endif