#include "interpreter.h"
#include "shellmemory.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

int shellUI()
{
    printf("Welcome to the Arian Omidi shell!\n"
           "Version 1.0 Created January 2021\n");

    shell_memory_initialize();

    while (!feof(stdin)) {
        char *line = NULL;
        size_t linecap = 0;
	
	printf("$ ");
        fflush(stdout);

        if (getline(&line, &linecap, stdin) == -1) {
		// Redirect input to terminal if file is done
		if (!isatty(STDIN_FILENO)){
                	if (!freopen("/dev/tty", "r", stdin)) {
                        	perror("/dev/tty");
                        	exit(1);
                	}
			getline(&line, &linecap, stdin);
		} else {
			break;
		}
        }

        (void)interpret(line);
        free(line);
    }

    shell_memory_destory();
    return 0;
}
