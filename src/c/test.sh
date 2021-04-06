#!/bin/bash

# compile
./compile.sh

# run test scripts from test/ dir
./mykernel < test/testfile1.txt > test/output.txt
./mykernel < test/testfile2.txt >> test/output.txt
./mykernel < test/testfile3.txt >> test/output.txt

# test output to expected output
echo '========== RUN TEST =========='
echo -e '    - test/testfile1.txt'
echo -e '    - test/testfile2.txt'
echo -e '    - test/testfile3.txt'
cmp --silent test/expected-output.txt test/output.txt && echo -e '========= TEST PASSED =========' || ( echo -e '========= TEST FAILED =========\n"diff test/expected-output.txt test/output.txt"' && diff test/expected-output.txt test/output.txt )

exit