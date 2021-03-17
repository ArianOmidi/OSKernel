#!/bin/sh

gcc -c cpu.c interpreter.c kernel.c pcb.c ram.c shell.c shellmemory.c memorymanager.c
gcc -o mykernel cpu.o interpreter.o kernel.o pcb.o ram.o shell.o shellmemory.o memorymanager.o
