#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ram.h"

#define PAGE_SIZE 4

int programNum = 0;

// -- HELPER METHODS -- //

FILE *copyFile(FILE *src, char *path) {
  char programName[200];
  sprintf(programName, "%s/program%d.txt", path, ++programNum);

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
  int lines = 0;

  // Get total number of lines
  rewind(f);
  char c = fgetc(f);
  while (c != EOF) {
    if (c == '\n') {
      lines++;
    }
    c = fgetc(f);
  }

  printf("\nnum of lines %d\n", lines);
  return lines / PAGE_SIZE + 1;
}

void loadPage(int pageNumber, FILE *f, int frameNumber) {
  char line[1000];
  int pageIndex = 0, frameIndex = 0;

  while (fgets(line, sizeof(line), f)) {
    if (pageIndex < pageNumber * PAGE_SIZE) {
      pageIndex++;
      continue;
    } else if (pageIndex >= (pageNumber + 1) * PAGE_SIZE) {
      break;
    }

    ram[frameNumber * PAGE_SIZE + frameIndex] = line;
    printf("%s", line);
    frameIndex++;
  }
}

int findFrame() {
  for (int i = 0; i < RAM_SIZE / PAGE_SIZE; i++) {
    if (ram[i * PAGE_SIZE] == NULL) return i;
  }
  return -1;
}

// -- MAIN LAUNCHER -- //

int launcher(FILE *p) {
  // Copy the entire file into the Backing Store
  FILE *copy = copyFile(p, "BackingStore");
  fclose(p);

  if (copy == NULL) return -5;

  printf("%d", countTotalPages(copy));

  return 0;
}