#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct PARTITION {
  char *path;
  int total_blocks;
  int block_size;
} aPartition;  // TODO: rename?

struct FAT {
  char *filename;     // name of the file in partition
  int file_length;    // length of file in number of blocks
  int blockPtrs[10];  // pointers to the blocks in the partition with data from
                      // the file(block numbers)
  int current_location;  // current location of the file pointer (local block//
                         // number)
} fat[20];

int next_free_fat_cell;

char *block_buffer;

FILE *active_file_table[5];

int map[5];

// Helper method headers
int findFreeCell();
int findFATMapping(int fat_index);
void setFileToBlock(FILE *f, int blockIndex);
int getFreeBlock();
int saveFS();
void printFAT();

// initialize all global data structure and variables to zero or null. Called
// from your boot() function.
void initIO() {
  aPartition.path = NULL;
  aPartition.total_blocks = 0;
  aPartition.block_size = 0;

  for (int i = 0; i < 20; i++) {
    fat[i].filename = NULL;
    fat[i].file_length = 0;
    fat[i].current_location = -1;
    for (int j = 0; j < 10; j++) {
      fat[i].blockPtrs[j] = -1;
    }
  }
  next_free_fat_cell = 0;

  for (int i = 0; i < 5; i++) map[i] = -1;
}

// create & format partition. Called from your mount() function that lives in
// the interpreter, associated to your scripting mount command.
int partition(char *name, int blocksize, int totalblocks) {
  // create PARTITION directory
  system("mkdir -p PARTITION");

  // create a file for the partition
  char partitionPath[100];
  sprintf(partitionPath, "PARTITION/%s.txt", name);

  FILE *f = fopen(partitionPath, "w+");
  if (f == NULL) return 0;

  // TODO: write partition info
  fprintf(f, "%d,", totalblocks);
  fprintf(f, "%d:", blocksize);

  // write info of FAT
  for (int i = 0; i < 20; i++) {
    if (i > 0) fputc(';', f);

    // filename and file length
    fprintf(f, "NULL,0");

    // data pointers
    for (int j = 0; j < 10; j++) {
      fprintf(f, ",%d", -1);
    }
  }
  fputc(':', f);

  // write totalblocks * blocksize '0's to file
  for (int i = 0; i < totalblocks * blocksize; i++) {
    fputc('0', f);
  }

  fflush(f);
  fclose(f);
  return 1;
}

// load FAT & create buffer_block. Called from your mount() function that lives
// in the interpreter, associated to your scripting mount command.
int mountFS(char *name) {
  int errorCode = saveFS();
  if (errorCode < 0) return 0;
  initIO();

  // create a file for the partition
  char partitionPath[100];
  sprintf(partitionPath, "PARTITION/%s.txt", name);
  aPartition.path = strdup(partitionPath);

  FILE *f = fopen(partitionPath, "r");
  if (f == NULL) return 0;

  // load partition info to aPartition
  fscanf(f, "%d,%d:", &aPartition.total_blocks, &aPartition.block_size);

  // load partition FAT table
  int i = 0;
  char c = fgetc(f);
  while (c != ':' && c != EOF) {
    if (c == ';') {
      c = fgetc(f);
      continue;
    }

    // get filename
    char filename[100];
    for (int j = 0; j < 100; j++) {
      if (c == ',') {
        filename[j] = '\0';
        break;
      }
      filename[j] = c;
      c = fgetc(f);
    }

    if (strcmp(filename, "NULL") == 0) {
      fat[i].filename = NULL;
      fat[i].current_location = 0;
    } else {
      fat[i].filename = strdup(filename);
      fat[i].current_location = 0;
      next_free_fat_cell = i + 1;
    }

    // get file length
    fscanf(f, "%d", &fat[i].file_length);

    // get file block pointers
    for (int j = 0; j < 10; j++) {
      fscanf(f, ",%d", &fat[i].blockPtrs[j]);
    }

    c = fgetc(f);
    i++;
  }
  fflush(f);

  // reset block_buffer size
  free(block_buffer);
  block_buffer = (char *)malloc(aPartition.block_size);

  // TODO: remove
  printFAT();

  fclose(f);
  return 1;
}

// find filename or creates file if it does not exist, returns file’s FAT index.
// Called from your scripting read and write commands in the interpreter.
int openfile(char *name) {
  int fatIndex = -1;
  for (int i = 0; i < 20; i++) {
    if (fat[i].filename != NULL && strcmp(name, fat[i].filename) == 0) {
      fatIndex = i;
      break;
    }
  }

  if (fatIndex == -1) {
    if (next_free_fat_cell == 20) return -1;
    if (getFreeBlock() == -1) return -1;
    // TODO: check if there is avaliable space on partition?

    fatIndex = next_free_fat_cell;
    // create a new entry in FAT
    fat[fatIndex].filename = strdup(name);
    fat[fatIndex].current_location = 0;

    // TODO: remove or redo as in assignment description
    // ------------------------------------------------- //
    // find available cell in active file table
    int aft_index = findFreeCell();
    if (aft_index == -1) return -1;  // ERROR if active file table is full

    // open the partition and seek to the first block.
    FILE *f = fopen(aPartition.path, "r+");
    if (f == NULL) return -3;

    active_file_table[aft_index] = f;
    map[aft_index] = fatIndex;
    // ------------------------------------------------- //

    next_free_fat_cell++;
  } else {
    // If fat is loaded in act return
    int fatMapping = findFATMapping(fatIndex);
    if (fatMapping != -1) return fatIndex;

    // find available cell in active file table
    int aft_index = findFreeCell();
    if (aft_index == -1) return -1;  // ERROR if active file table is full

    // open the partition and seek to the first block.
    FILE *f = fopen(aPartition.path, "r+");
    if (f == NULL) return -1;

    setFileToBlock(f, fat[fatIndex].blockPtrs[0]);

    // add file pointer to active_file_table and update map
    active_file_table[aft_index] = f;
    map[aft_index] = fatIndex;

    // set fat file cur location to 0
    fat[fatIndex].current_location = 0;
  }

  return fatIndex;
}

// using the file FAT index number, load buffer with data from current_location.
// Return block data as string from block_buffer.
char *readBlock(int file) {
  // check for invalid FAT index
  if (file < 0 || file >= 20) return NULL;

  // if at EOF, throw error
  if (fat[file].current_location == fat[file].file_length) return NULL;

  // if not at EOF, load the block_buffer with the data from the file
  FILE *f = active_file_table[findFATMapping(file)];
  if (f == NULL) return NULL;

  // set file pointer to start of cur block
  setFileToBlock(f, fat[file].blockPtrs[fat[file].current_location]);

  for (int i = 0; i < aPartition.block_size; i++) {
    block_buffer[i] = fgetc(f);
  }
  fat[file].current_location++;

  return strdup(block_buffer);
}

// using the file FAT index number, write data to disk at current_location
int writeBlock(int file, char *data) {
  // check for invalid FAT index
  if (file < 0 || file >= 20) return -1;

  // get corresponding file
  FILE *f = active_file_table[findFATMapping(file)];
  if (f == NULL) return -1;

  // if at EOF, get free block and set the fp to it
  int nextBlock;
  if (fat[file].current_location == fat[file].file_length) {
    nextBlock = getFreeBlock();
    printf(" -> FREE BLOCK: %d ", nextBlock);
    if (nextBlock == -1) return -1;
  } else {
    nextBlock = fat[file].blockPtrs[fat[file].current_location];
  }
  setFileToBlock(f, nextBlock);

  // write data
  for (int i = 0; i < aPartition.block_size; i++) {
    fputc(data[i], f);
  }
  fflush(f);

  // update FAT table
  if (fat[file].current_location == fat[file].file_length)
    fat[file].file_length++;

  fat[file].blockPtrs[fat[file].current_location] = nextBlock;
  fat[file].current_location++;

  return 0;
}

/* --- HELPER METHODS --- */

int findFreeCell() {
  for (int i = 0; i < 5; i++) {
    if (active_file_table[i] == NULL) return i;
  }
  return -1;
}

int findFATMapping(int fat_index) {
  for (int i = 0; i < 5; i++) {
    if (map[i] == fat_index) return i;
  }
  return -1;
}

void setFileToBlock(FILE *f, int blockIndex) {
  // Set to start of data section and see
  fseek(f, -aPartition.total_blocks * aPartition.block_size, SEEK_END);
  fseek(f, blockIndex * aPartition.block_size, SEEK_CUR);
}

int getFreeBlock() {
  FILE *f = fopen(aPartition.path, "r");
  int c;
  int numOfZeros;

  // Set to start of data section and see
  fseek(f, -aPartition.total_blocks * aPartition.block_size, SEEK_END);

  printf("DATA:");
  for (int i = 0; i < aPartition.total_blocks; i++) {
    printf(" ");
    numOfZeros = 0;
    for (int j = 0; j < aPartition.block_size; j++) {
      c = fgetc(f);
      if (c == '0') numOfZeros++;
      printf("%c", c);
    }

    if (numOfZeros == aPartition.block_size) {
      fclose(f);
      return i;
    }
  }

  fclose(f);
  return -1;
}

int getBlockSize() { return aPartition.block_size; }

int saveFS() {
  if (aPartition.path == NULL) return 0;

  // Close all files
  for (int i = 0; i < 5; i++) {
    if (active_file_table[i] != NULL) fclose(active_file_table[i]);
  }

  // Get data
  FILE *f = fopen(aPartition.path, "r");
  if (f == NULL) return -1;

  char data[aPartition.total_blocks * aPartition.block_size];
  fseek(f, -aPartition.total_blocks * aPartition.block_size, SEEK_END);
  for (int i = 0; i < aPartition.total_blocks * aPartition.block_size; i++) {
    data[i] = fgetc(f);
  }
  fclose(f);

  // Write to file
  f = fopen(aPartition.path, "w+");
  if (f == NULL) return -1;

  // write partition info
  fprintf(f, "%d,", aPartition.total_blocks);
  fprintf(f, "%d:", aPartition.block_size);

  // write info of FAT
  for (int i = 0; i < 20; i++) {
    if (i > 0) fputc(';', f);

    // filename
    if (fat[i].filename == NULL)
      fprintf(f, "NULL");
    else
      fprintf(f, "%s", fat[i].filename);

    // file length
    fprintf(f, ",%d", fat[i].file_length);

    // data pointers
    for (int j = 0; j < 10; j++) {
      fprintf(f, ",%d", fat[i].blockPtrs[j]);
    }
  }

  // write data
  fprintf(f, ":%s", data);

  fflush(f);
  fclose(f);
  return 0;
}

void printFAT() {
  printf(
      "\n\t------------------------------ FAT "
      "----------------------------\n");

  for (int i = 0; i < 20; i++) {
    printf("\t%d ->\t\"%s\" - %d / %d\n", i, fat[i].filename,
           fat[i].current_location, fat[i].file_length);
    printf("\t|");
    for (int j = 0; j < 10; j++) printf("%d|", fat[i].blockPtrs[j]);
    printf("\n");
  }
  printf(
      "\t--------------------------------------------------------------\n\n");
}