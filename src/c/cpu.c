#include "ram.h"
#include "interpreter.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct CPU {
	int IP;
	char IR[1000];
	int quanta;
} CPU;

CPU* cpu;

void initCPU(){
	cpu = (CPU*) malloc(sizeof(CPU));
	cpu->quanta = 2;	
}

void setCPU(int IP){
	cpu->IP = IP;
	strcpy(cpu->IR, loadFromRAM(IP));
}

int runCPU(int quanta, int end){
	for (int i = 0; i < quanta; i++){
		if (cpu->IP > end){
			printf("Program Finished\n");
			return -1;
		}
		
		printf(" - %s", loadFromRAM(cpu->IP));

		strcpy(cpu->IR, loadFromRAM(cpu->IP));
		interpret(cpu->IR);

		cpu->IP = cpu->IP + 1; 
	}

	return 0;
}

