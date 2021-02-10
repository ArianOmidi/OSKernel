#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Defining Constants
#define RAM_SPACE 1000

char *ram[RAM_SPACE];
int nextFreeCell;


void clearRAM(int start, int end){
	for (int i = start; i <= end; i++){
		ram[i] = NULL;
	}

	nextFreeCell = start;
}

void addToRAM(FILE *p, int *start, int *end){
	char buffer[1000];
	int k = nextFreeCell;

	*start = nextFreeCell;

	while (fgets(buffer, 999, p) != NULL){
		if (k < RAM_SPACE && ram[k] == NULL) {
			ram[k] = strdup(buffer);
			k++;
		} else {
			printf("ERROR: Not enough RAM to add program.\n");
			clearRAM(0, k - 1);
			*end = -1;
			return;
		}	
    	}

	nextFreeCell = k;
	*end = k - 1;
}

void emptyRAM(){
	clearRAM(0, RAM_SPACE - 1);
}

char* loadFromRAM(int address){
	if (address >= 0 && address < 1000)
		return ram[address];

	printf("ERROR: Address not in RAM\n");
	return NULL;
}
