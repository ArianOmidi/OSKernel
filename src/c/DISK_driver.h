#ifndef DISK_DRIVER_H
#define DISK_DRIVER_H

void initIO();

int partition(char *name, int blocksize, int totalblocks);

int mountFS(char *name);

int openfile(char *name);

char *readBlock(int file);

int writeBlock(int file, char *data);

int saveFS();

#endif