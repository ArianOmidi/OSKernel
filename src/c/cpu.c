#include <string.h>

#include "interpreter.h"
#include "pcb.h"
#include "ram.h"
#include "shell.h"

#define DEFAULT_QUANTA 2

struct CPU {
  int IP;
  char IR[1000];
  int offest;
  int quanta;
} CPU;

/*
This method passes a quanta.
It will then executes quanta number of lines in RAM as from the address
pointed by the CPU IP.
Returns an errorCode.
*/
int run(int quanta) {
  // If a quanta greater than the default quanta of 2 is passed,
  // run will instead execute only default quanta of lines.
  if (quanta > DEFAULT_QUANTA) {
    quanta = DEFAULT_QUANTA;
  }

  for (int i = 0; i < quanta; i++) {
    if (ram[CPU.IP + CPU.offest] == NULL) return -10;

    strcpy(CPU.IR, ram[CPU.IP + CPU.offest]);
    int errorCode = parse(CPU.IR);
    // Do error checking and if error, return error
    if (errorCode != 0) {
      // Display error message if fatal
      if (errorCode < 0) {
        ram[CPU.IP + CPU.offest][strlen(ram[CPU.IP + CPU.offest]) - 2] = '\0';
        displayCode(errorCode, ram[CPU.IP + CPU.offest]);
      }
      return errorCode;
    }

    CPU.offest++;
    // If offset is greater than 4, throw page fault
    if (CPU.offest >= 4) return 10;
  }
  return 0;
}
