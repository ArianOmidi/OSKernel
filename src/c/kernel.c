#include "shell.h"
#include "ram.h"
#include "pcb.h"
#include "cpu.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
	// Init RAM
	emptyRAM();

	printf("Kernel 1.0 loaded!\n");
	shellUI();
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


