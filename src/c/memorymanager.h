#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

#include <stdio.h>

#include "pcb.h"

#define PAGE_SIZE 4

int launcher(FILE *p);

void initFrameQueue();

int freeFrames(PCB *pcb);

void handlePageFault(PCB *pcb);

#endif