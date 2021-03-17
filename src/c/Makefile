CC=gcc
CFLAGS=-g

mykernel : cpu.o interpreter.o kernel.o pcb.o ram.o shell.o shellmemory.o memorymanager.o
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

.PHONY:
clean:
	rm -f *.o
	rm -f mykernel

.PHONY:
test_a1: mykernel
	./mykernel < ../../test/a1/TESTFILE.txt > ../../test/a1/output.txt
	diff ../../test/a1/output.txt ../../test/a1/EXPECTED_OUTPUT.txt
	rm ../../test/a1/output.txt

.PHONY:
test_a2: mykernel
	./mykernel < ../../test/a2/TESTFILE.txt > ../../test/a2/output.txt
	diff ../../test/a2/output.txt ../../test/a2/EXPECTED_OUTPUT.txt
	rm ../../test/a2/output.txt

.PHONY:
test_a3: mykernel
	./mykernel < ../../test/a3/TESTFILE.txt > ../../test/a3/output.txt
	diff ../../test/a3/output.txt ../../test/a3/EXPECTED_OUTPUT.txt
	rm ../../test/a3/output.txt

.PHONY:
test: test_a1 test_a2 test_a3
	

