#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct PARTITION {
  int total_blocks;
  int block_size;
} aPartition;  // TODO: rename?

struct FAT {
  char *filename;
  int file_length;
  int blockPtrs[10];
  int current_location;
} fat[20];

char *block_buffer;

FILE *active_file_table[5];

// initialize all global data structure and variables to zero or null. Called
// from your boot() function.
void initIO();

// create & format partition. Called from your mount() function that lives in
// the interpreter, associated to your scripting mount command.
int partition(char *name, int blocksize, int totalblocks);

// load FAT & create buffer_block. Called from your mount() function that lives
// in the interpreter, associated to your scripting mount command.
int mountFS(char *name);

// find filename or creates file if it does not exist, returns file’s FAT index.
// Called from your scripting read and write commands in the interpreter.
int openfile(char *name);

// using the file FAT index number, load buffer with data from current_location.
// Return block data as string from block_buffer.
char *readBlock(int file);

// using the file FAT index number, write data to disk at current_location
int writeBlock(int file, char *data);