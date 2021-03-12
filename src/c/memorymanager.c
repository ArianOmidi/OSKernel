#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kernel.h"
#include "pcb.h"
#include "ram.h"

#define PAGE_SIZE 4

int programId = 0;

/*
FIFO Free Frame Queue and functions
*/
typedef struct Frame {
  int frameNumber;
  struct Frame *next;
} Frame;

typedef struct FrameQueue {
  Frame *head;
  int size;
} FrameQueue;

FrameQueue *frameQueue;

void addFreeFrame(int i) {
  Frame *tmp = (Frame *)malloc(sizeof(Frame));
  tmp->frameNumber = i;

  tmp->next = frameQueue->head;
  frameQueue->head = tmp;
  frameQueue->size++;
}

void initFrameQueue() {
  // init the Frame Queue
  frameQueue = (FrameQueue *)malloc(sizeof(FrameQueue));
  frameQueue->head = NULL;
  frameQueue->size = 0;

  // Add free frames to queue
  for (int i = 0; i < RAM_SIZE / PAGE_SIZE; i++) {
    addFreeFrame(i);
  }
}

void freeFrames(PCB *pcb) {
  for (int i = 0; i < pcb->pages_max; i++) {
    if (pcb->pageTable[i] != -1) {
      addFreeFrame(pcb->pageTable[i]);
      pcb->pageTable[i] = -1;
    }
  }
}

int findFrame() {
  Frame *tmp = frameQueue->head;

  if (tmp == NULL) return -1;

  frameQueue->head = frameQueue->head->next;
  frameQueue->size--;
  return tmp->frameNumber;
}

// -- HELPER METHODS -- //

FILE *copyFile(FILE *src, char *path) {
  char programName[200];
  sprintf(programName, "%s/program%d.txt", path, ++programId);

  FILE *copy = fopen(programName, "w+");
  if (copy == NULL) return NULL;

  // Copy src contents
  char c = fgetc(src);
  while (c != EOF) {
    fputc(c, copy);
    c = fgetc(src);
  }

  return copy;
}

int countTotalPages(FILE *f) {
  int numOfLines = 0;

  // Get total number of lines
  rewind(f);
  char c = fgetc(f);
  while (c != EOF) {
    if (c == '\n') {
      numOfLines++;
    }
    c = fgetc(f);
  }

  printf("\nnum of lines %d\n", numOfLines);
  return numOfLines / PAGE_SIZE + 1;
}

void loadPage(int pageNumber, FILE *f, int frameNumber) {
  char buffer[1000];
  int pageIndex = 0, frameIndex = 0;

  rewind(f);
  while (fgets(buffer, sizeof(buffer), f)) {
    if (pageIndex < pageNumber * PAGE_SIZE) {
      pageIndex++;
      continue;
    } else if (pageIndex < (pageNumber + 1) * PAGE_SIZE) {
      ram[frameNumber * PAGE_SIZE + frameIndex] = strdup(buffer);
      frameIndex++;
      pageIndex++;
    } else {
      break;
    }
  }
}

int findVictim(PCB *p) {
  int randomFrame = rand() % (RAM_SIZE / PAGE_SIZE);
  int counter = 0;
  int isValidPage;

  do {
    counter++;
    isValidPage = 0;
    for (int i = 0; i < (RAM_SIZE / PAGE_SIZE); i++) {
      if (p->pageTable[i] == randomFrame) {
        randomFrame = (randomFrame + 1) % (RAM_SIZE / PAGE_SIZE);
        isValidPage = 1;
        break;
      }
    }
  } while (isValidPage == 1 && counter <= (RAM_SIZE / PAGE_SIZE));

  return randomFrame;
}

void updatePageTable(PCB *p, int pageNumber, int frameNumber, int victimFrame) {
  if (frameNumber >= 0) {
    p->pageTable[pageNumber] = frameNumber;
  } else {
    p->pageTable[pageNumber] = victimFrame;
    // TODO: update victim pcb pageTable
  }
}

int handlePageFault(PCB *pcb) {
  int frameNum, victimFrame;
  char programName[200];
  sprintf(programName, "BackingStore/program%d.txt", pcb->PID);

  FILE *f = fopen(programName, "r");
  if (f == NULL) return -3;

  frameNum = findFrame();
  if (frameNum == -1) {
    victimFrame = findVictim(pcb);
    loadPage(pcb->PC_page, f, victimFrame);
    updatePageTable(pcb, pcb->PC_page, -1, victimFrame);
  } else {
    loadPage(pcb->PC_page, f, frameNum);
    updatePageTable(pcb, pcb->PC_page, frameNum, -1);
  }

  return 0;
}

// -- MAIN LAUNCHER -- //

int launcher(FILE *p) {
  // Copy the entire file into the Backing Store
  FILE *f = copyFile(p, "BackingStore");
  fclose(p);

  // Check if copy was created
  if (f == NULL) return 0;

  // Create a PCB and add to ReadyList
  PCB *pcb = makePCB(programId, countTotalPages(f));

  printPCB(pcb);
  printRAM();

  addToReady(pcb);

  // Load 2 Pages
  int frameNum, victimFrame;
  for (int i = 0; i < pcb->pages_max && i < 2; i++) {
    frameNum = findFrame();
    if (frameNum == -1) {
      victimFrame = findVictim(pcb);
      loadPage(i, f, victimFrame);
      updatePageTable(pcb, i, -1, victimFrame);
    } else {
      loadPage(i, f, frameNum);
      updatePageTable(pcb, i, frameNum, -1);
    }
  }

  printPCB(pcb);
  printRAM();

  return 1;
}