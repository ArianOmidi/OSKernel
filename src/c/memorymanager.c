#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int programNum = 0;

FILE *copyFile(FILE *src, char *path) {
  char programName[200];
  sprintf(programName, "%s/program%d.txt", path, ++programNum);

  FILE *copy = fopen(programName, "w");
  if (copy == NULL) return NULL;

  // Copy src contents
  char c = fgetc(src);
  while (c != EOF) {
    fputc(c, copy);
    c = fgetc(src);
  }

  fclose(src);
  fclose(copy);

  return copy;
}

int launcher(FILE *p) {
  // Copy the entire file into the Backing Store
  FILE *copy = copyFile(p, "BackingStore");
  fclose(p);

  if (copy == NULL) return -1;

  return 0;
}

// -- HELPER METHODS -- //

int countTotalPages(FILE *f) {}