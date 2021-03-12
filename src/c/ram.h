#include <stdio.h>

#define RAM_SIZE 40

/*
Ram structure implemented as an array.
Size is RAM_SIZE strings
*/
char *ram[RAM_SIZE];

/*
This function will add the content of a file to the local ram array variable
In the case of an error (due to lack of RAM), -1 will be assigned to
the values pointed by start and end. Use this to check for errors.
p : file pointer
start : pointer to variable which will store the index of first line of file in
ram end : pointer to variable which will store the index of last line of file in
ram
*/
void addToRAM(FILE *, int *, int *);

/*
This function will delete the content in ram between the 2 indices parameters
(inclusive) start : pointer to variable which will store the index of first line
of file in ram end : pointer to variable which will store the index of last line
of file in ram
*/
void removeFromRam(int, int);

/*
Reset the pointer to the free cell back to index 0
*/
void resetRAM();

/*
Init RAM
*/
void initRAM();

/*
Get next free frame
*/
int getFreeFrame();
