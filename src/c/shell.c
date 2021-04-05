#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "interpreter.h"

/*
This functions passes an int errorCode and command string and display the
appropriate error message for that errorCode
ERRORCODE -1 : RAN OUT OF SHELL MEMORY
ERRORCODE -2 : INCORRECT NUMBER OF ARGUMENTS
ERRORCODE -3 : SCRIPT NOT FOUND
ERRORCODE -4 : UNKNOWN COMMAND.
ERRORCODE -5 : NOT ENOUGH RAM (EXEC)
ERRORCODE -6 : LAUNCHING ERROR (EXEC)
*/
void displayCode(int errorCode, char *command) {
  switch (errorCode) {
    case -1:
      printf("ERRORCODE -1 : RAN OUT OF SHELL MEMORY\n");
      break;
    case -2:
      printf("ERRORCODE -2 : '%s' INCORRECT NUMBER OF ARGUMENTS. TRY 'help'\n",
             command);
      break;
    case -3:
      printf("ERRORCODE -3 : SCRIPT NOT FOUND\n");
      break;
    case -4:
      printf("ERRORCODE -4 : '%s' IS AN UNKNOWN COMMAND. TRY 'help'\n",
             command);
      break;
    case -5:
      printf("ERRORCODE -5 : NOT ENOUGH RAM TO ADD PROGRAM.'%s'\n", command);
      break;
    case -6:
      printf("ERRORCODE -6 : LAUNCHING ERROR\n");
      break;
    case -7:
      printf("ERRORCODE -7 : MALFORMED ARGUMENTS: %s\n", command);
      break;
    case -10:
      printf("ERRORCODE -10 : FID IS INVALID IN CURRENT PARTITION\n");
      break;
    case -11:
      printf("ERRORCODE -11 : CURRENT PARTITION DATA IS FULL\n");
      break;
    case -12:
      printf("ERRORCODE -12 : CURRENT PARTITION FAT IS FULL\n");
      break;
    case -13:
      printf(
          "ERRORCODE -13 : CURRENT PARTITION ACTIVE FILE TABLE IS FULL - "
          "REMOUNT PARITION\n");
      break;
    case -14:
      printf("ERRORCODE -14 : FAILED TO MOUNT PARTITION '%s'\n", command);
      break;
    case -15:
      printf("ERRORCODE -15 : FAILED TO CREATE PARTITION '%s'\n", command);
      break;
    case -16:
      printf("WARNING : Requested to read past EOF\n");
      break;
  }

  return;
}

int parse(char ui[]) {
  // printf("In Parser: '%s'",ui);
  char tmp[200];
  int a, b;
  char *words[10];
  int w = 0;  // wordsIdx
  for (int i = 0; i < 10; i++) {
    words[i] = "_NONE_";
  }
  for (a = 0; ui[a] == ' ' && a < 1000; a++)
    ;  // skip white spaces
  while (ui[a] != '\0' && ui[a] != '\n' && a < 1000) {
    for (b = 0; ui[a] != '\0' && ui[a] != '\n' && ui[a] != '\r' &&
                ui[a] != ' ' && a < 1000;
         a++, b++)
      tmp[b] = ui[a];  // extract a word
    tmp[b] = '\0';
    words[w] = strdup(tmp);
    if (ui[a] == '\0' || ui[a] == '\n' || ui[a] == '\r') break;
    for (; ui[a] == ' ' && a < 1000; a++)
      ;  // skip white spaces
    w++;
  }

  int errorCode = interpreter(words);
  if (errorCode < 0) displayCode(errorCode, words[0]);

  return errorCode;
}

/*
This is the main loop where
users gets to use the shell.
*/
int shellUI() {
  printf(
      "----------------------------------\nKernel 4.0 loaded!\nWelcome to "
      "the Arian Omidi Shell!\nShell Version 3.0 Updated April "
      "2021\n----------------------------------\n");
  char prompt[15] = {'$', ' ', '\0'};
  char userinput[1000];
  int errorCode;

  while (1) {
    while (!feof(stdin)) {
      // prints prompt if input is from the terminal
      if (isatty(STDIN_FILENO)) printf("%s", prompt);
      // gets user input
      fgets(userinput, 999, stdin);
      // parses and interprets the command
      errorCode = parse(userinput);
      // If the user entered the "quit" command
      if (errorCode == 1) {
        printf(
            "----------------------------------\nExiting, "
            "Farewell...\n----------------------------------\n");
        return 0;
      }
    }
    // Redirect input to terminal if file is done
    if (!isatty(STDIN_FILENO)) {
      if (!freopen("/dev/tty", "r", stdin)) {
        perror("/dev/tty");
        exit(1);
      }
    }
  }
  return 0;
}
