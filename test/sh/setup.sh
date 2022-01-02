#!/bin/bash

# If number of arguments less then 1; print usage and exit
if [ $# -lt 2 ]; then
    printf "Usage: %s <path> <script>\n" "$0" >&2
    exit 1
fi 

# check if compliation file exists
if [[ ! -d "$1" ]]; then
    printf "ERROR : Directory '%s' does not exist\n" "$1" 
    exit 1
elif [[ ! -f "$1"/"$2" ]]; then
    printf "ERROR : Script '%s' does not exist\n" "$2" 
    exit 1
else 
    cd "$1"
    ./"$2"
    cd - > /dev/null
fi

exit 0