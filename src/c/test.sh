#!/bin/bash

# compile
./compile.sh

# run test scripts from test/ dir
./mykernel < test/testfile1.txt
./mykernel < test/testfile2.txt
./mykernel < test/testfile3.txt

exit