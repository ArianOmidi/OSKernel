#include "ram.h"
#include "interpreter.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define QUANTA 2

typedef struct CPU {
	int IP;
	char IR[1000];
	int quanta;
} CPU;

CPU* cpu;

void initCPU(){
	cpu = (CPU*) malloc(sizeof(CPU));
	cpu->quanta = QUANTA;	
}

int getQuanta(){
	return QUANTA;
}

void setCPU(int IP){
	cpu->IP = IP;
	strcpy(cpu->IR, loadFromRAM(IP));
}

int run(int quanta){
	for (int i = 0; i < quanta; i++){
		strcpy(cpu->IR, loadFromRAM(cpu->IP));
		interpret(cpu->IR);
		cpu->IP++;
	}

	return 0;
}

