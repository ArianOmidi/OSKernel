#!/bin/bash

# If number of arguments less then 1; print usage and exit
if [ $# -lt 1 ]; then
    printf "Usage: %s <application> [<location>]\n" "$0" >&2
    exit 1
fi 

bin="$1"           # The application (from command arg)
diff="diff -iad"   # Diff command, or what ever
test_list="tests.csv"
ext="txt"

PASS_COLOR='\033[0;32m'
FAIL_COLOR='\033[0;31m'
NO_COLOR='\033[0m' 

# test file directory
if [ $# -ge 2 ]; then
    dir="$2"
    curdir=$(pwd)
else
    dir="."
    curdir="."
fi

# check if directory exists
if [[ ! -d "$dir" ]]; then
    printf "ERROR : Directory '%s' does not exist\n" "$dir" "$file_in"
    exit 2
else 
    cd "$dir"
fi

# check if test list exists
if [ ! -f "$test_list" ]; then
    printf "ERROR : Test list '%s/%s' does not exist\n" "$dir" "$test_list"
    exit 3
fi

# add tests from test list to file base
file_base=()
while IFS= read -r line 
do
    file_base+=("$line")
done < "$test_list"


# Loop the array
for file in "${file_base[@]}"; do
    echo "-------------------------------------"
    # Padd file_base with suffixes
    file_in="$file.$ext"             # The in file
    file_out_val="$file-expected.$ext"       # The out file to check against
    file_out_tst="$file-out.$ext"   # The outfile from test application

    # Validate infile exists (do the same for out validate file)
    if [ ! -f "$file_in" ]; then
        printf "ERROR : Input file '%s/%s' does not exist\n" "$dir" "$file_in"
        continue;
    fi
    if [ ! -f "$file_out_val" ]; then
        printf "ERROR : Validation file '%s/%s' does not exist\n" "$dir" "$file_out_val"
        continue;
    fi

    # print the test performed
    printf "> %s/%s\t" "$dir" "$file_in"
    
    # Run application, redirect in file to app, and output to out file
    "./$bin" < "$file_in" > "$file_out_tst"

    # Execute diff
    output=$($diff "$file_out_val" "$file_out_tst")

    # Check exit code from previous command (ie diff)
    # We need to add this to a variable else we can't print it
    # as it will be changed by the if [
    # Iff not 0 then the files differ (at least with diff)
    e_code=$?
    if [ $e_code != 0 ]; then
            printf "${FAIL_COLOR}FAILED${NO_COLOR}\n"
            printf "%s\n" "$output"
    else
            printf "${PASS_COLOR}PASSED${NO_COLOR}\n"
    fi

    # # Pause by prompt
    # read -p "Enter a to abort, anything else to continue: " input_data
    # # Iff input is "a" then abort
    # [ "$input_data" == "a" ] && break
done
echo "-------------------------------------"

cd "$curdir"

# Clean exit with status 0
exit 0