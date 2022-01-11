# OS Kernel

<!-- ABOUT THE PROJECT -->
## About

A fully functioning OS Kernel and file system written in C. Complete with a contigous file system, disk drivers, I/O, memory manager, CPU emulation, and more!

<!-- GETTING STARTED -->
## Getting Started

1. Clone the repo
   ```sh
   git clone https://github.com/ArianOmidi/OSKernel.git
   ```
2. Compile Project
   ```sh
   cd ./src/c/
   make
   ```
3. Test Files
   ```sh
   ./mykernel < testfile.txt
   ```
4. Test Suite (Using [BASH Testing System](https://github.com/ArianOmidi/BashTestingSystem))
   ```sh
   cd ./test/
   ./test.sh
   ```

<!-- USAGE EXAMPLES -->
## Usage

To start the kernel and create a shell, run `./mykernel`.

Run `help` once in the kernel to get a list of available commands.
```
=======================================================================================================
COMMANDS			DESCRIPTIONS
=======================================================================================================
help				| Displays all commands.
-------------------------------------------------------------------------------------------------------
quit				| Terminates the shell.
-------------------------------------------------------------------------------------------------------
set VAR STRING			| Assigns the value STRING to the shell memory variable, VAR.
-------------------------------------------------------------------------------------------------------
print VAR			| Displays the STRING value assigned to the shell memory variable, VAR.
-------------------------------------------------------------------------------------------------------
run SCRIPT.TXT			| Executes the file SCRIPT.txt.
-------------------------------------------------------------------------------------------------------
exec P1 P2 P3			| Executes programs P1 P2 P3 concurrently.
-------------------------------------------------------------------------------------------------------
mount NAME BLOCKS BLOCK_SIZE	| Mounts an existing partition orcreates a new partition if the
				| partition name does not exist.
-------------------------------------------------------------------------------------------------------
write FILENAME [STRING]		| Writes the inputed string, enclosed by [], to a file.
-------------------------------------------------------------------------------------------------------
read FILENAME VAR		| Reads the contents of a file into the shell variable, VAR.
=======================================================================================================
```

<!-- CONTACT -->
## Contact

Arian Omidi - arian.omidi@icloud.com

***Any ideas or improvements are much appreciated.***
