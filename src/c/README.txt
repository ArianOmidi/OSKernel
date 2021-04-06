*** Implemented an Non-Contiguous File System ***

Compilation:
	- Makefile included -> run cmd "make" to compile the code
  - Alternative: compile script included -> run cmd "./compile.sh" to compile the code

Test Scripts:
	- Test scripts located in "tests" directory -> testfile1.txt, testfile2.txt, testfile3.txt
		- run w/ "./mykernel < tests/testfile[n].txt"

Test Suite:
  - Run test suite to run all testfiles, testing partition persistence
    - run w/ "./test.sh" 
  - Expected output located in "test/expected-output.txt"