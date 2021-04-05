CC=gcc
CFLAGS=-g

mykernel : cpu.o interpreter.o kernel.o pcb.o ram.o shell.o shellmemory.o memorymanager.o DISK_driver.o
	$(CC) $(CFLAGS) -o mykernel *.o

cpu.o : cpu.c 
	$(CC) -c $(CFLAGS) cpu.c

interpreter.o : interpreter.c
	$(CC) -c $(CFLAGS) interpreter.c 

kernel.o : kernel.c 
	$(CC) -c $(CFLAGS) kernel.c 

pcb.o : pcb.c 
	$(CC) -c $(CFLAGS) pcb.c 

ram.o : ram.c 
	$(CC) -c $(CFLAGS) ram.c 

shell.o : shell.c
	$(CC) -c $(CFLAGS) shell.c 

shellmemory.o : shellmemory.c
	$(CC) -c $(CFLAGS) shellmemory.c 

memorymanager.o : memorymanager.c
	$(CC) -c $(CFLAGS) memorymanager.c 

DISK_driver.o : DISK_driver.c
	$(CC) -c $(CFLAGS) DISK_driver.c 

.PHONY:
clean:
	rm -f *.o
	rm -f mykernel
	rm -fr BackingStore
	rm -fr PARTITION

.PHONY:
test: mykernel
	./mykernel < TESTFILE.txt