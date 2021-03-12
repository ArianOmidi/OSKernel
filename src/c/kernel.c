#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "cpu.h"
#include "interpreter.h"
#include "memorymanager.h"
#include "pcb.h"
#include "ram.h"
#include "shell.h"

/*
This is a node in the Ready Queue implemented as
a linked list.
A node holds a PCB pointer and a pointer to the next node.
PCB: PCB
next: next node
*/
typedef struct ReadyQueueNode {
  PCB* PCB;
  struct ReadyQueueNode* next;
} ReadyQueueNode;

ReadyQueueNode* head = NULL;
ReadyQueueNode* tail = NULL;
int sizeOfQueue = 0;

/*
Boot Sequence
*/
void boot() {
  // Init RAM and FIFO Frame Queue
  initRAM();
  initFrameQueue();

  // Init Backing Store
  system("rm -r BackingStore");
  system("mkdir BackingStore");
}

/*
Kernel Procedure
*/
int kernel() { return shellUI(); }

int main(int argc, char const* argv[]) {
  int error = 0;
  boot();            // First : actions performed by boot
  error = kernel();  // Second: actions performed by kernel
  return error;
}

/*
Adds a pcb to the tail of the linked list
*/
void addToReady(struct PCB* pcb) {
  ReadyQueueNode* newNode = (ReadyQueueNode*)malloc(sizeof(ReadyQueueNode));
  newNode->PCB = pcb;
  newNode->next = NULL;
  if (head == NULL) {
    head = newNode;
    tail = newNode;
  } else {
    tail->next = newNode;
    tail = newNode;
  }
  sizeOfQueue++;
}

/*
Returns the size of the queue
*/
int size() { return sizeOfQueue; }

/*
Pops the pcb at the head of the linked list.
pop will cause an error if linkedlist is empty.
Always check size of queue using size()
*/
struct PCB* pop() {
  PCB* topNode = head->PCB;
  ReadyQueueNode* temp = head;
  if (head == tail) {
    head = NULL;
    tail = NULL;
  } else {
    head = head->next;
  }
  free(temp);
  sizeOfQueue--;
  return topNode;
}

/*
Passes a filename
Opens the file, copies the content in the RAM.
Creates a PCB for that program.
Adds the PCB on the ready queue.
Return an errorCode:
ERRORCODE 0 : NO ERROR
ERRORCODE -3 : SCRIPT NOT FOUND
ERRORCODE -5 : NOT ENOUGH RAM (EXEC)
*/
int myinit(char* filename) {
  // Open the filename to get FILE *
  // call addToRam on that File *
  // If error (check via start/end variable), return that error
  // Else create pcb using MakePCB
  // Then add it to the ReadyQueue

  // TODO: remover
  // FILE* fp = fopen(filename, "r");
  // if (fp == NULL) return -3;
  // int start;
  // int end;
  // addToRAM(fp, &start, &end);
  // fclose(fp);
  // if (start == -1) return -5;
  // PCB* pcb = makePCB(start, end);
  // addToReady(pcb);
  return 0;
}

int scheduler() {
  // set CPU quanta to default, IP to -1, IR = NULL
  CPU.quanta = DEFAULT_QUANTA;
  CPU.IP = -1;

  while (size() != 0) {
    // pop head of queue
    PCB* pcb = pop();
    // get IP of CPU by looking up the frame in the PCB page table
    // and adding the PC_offset of the PCB
    CPU.IP = pcb->pageTable[pcb->PC_page] * PAGE_SIZE;
    CPU.offest = pcb->PC_offset;

    // TODO: TESTING - remove
    // printRAM();
    // printPCB(pcb);
    // printf("--> CPU - IP: %d, offset: %d\n", CPU.IP, CPU.offest);
    if (pcb->pageTable[pcb->PC_page] < 0) {
      printRAM();
      printPCB(pcb);
      printf("ERROR: PAGE NOT IN PAGE TABLE\n");
      exit(1);
    }
    //

    int isOver = FALSE;
    int remaining = (pcb->pages_max + 1) * PAGE_SIZE - pcb->PC -
                    1;  // TODO: calc remaining instructions
    int quanta = DEFAULT_QUANTA;

    if (DEFAULT_QUANTA >= remaining) {
      isOver = TRUE;
      quanta = remaining;
    }

    int errorCode = run(quanta);

    // Page Fault Handling
    if (errorCode == 10) {
      pcb->PC_page++;

      if (pcb->PC_page >= pcb->pages_max) {
        freeFrames(pcb);
        free(pcb);
      } else {
        if (pcb->pageTable[pcb->PC_page] == -1) {
          handlePageFault(pcb);
        }

        pcb->PC = pcb->PC_page * PAGE_SIZE;
        pcb->PC_offset = 0;
        addToReady(pcb);
      }
    } else if (errorCode < 0 || isOver) {
      // TODO: test
      freeFrames(pcb);
      free(pcb);
    } else {
      pcb->PC += DEFAULT_QUANTA;
      pcb->PC_offset += DEFAULT_QUANTA;
      addToReady(pcb);
    }
  }
  // reset RAM and FIFO Queue
  resetRAM();
  initFrameQueue();
  return 0;
}

/*
Flushes every pcb off the ready queue in the case of a load error
*/
void emptyReadyQueue() {
  while (head != NULL) {
    ReadyQueueNode* temp = head;
    head = head->next;
    free(temp->PCB);
    free(temp);
  }
  sizeOfQueue = 0;
}
