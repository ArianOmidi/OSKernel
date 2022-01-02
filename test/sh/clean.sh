#!/bin/bash

# If number of arguments less then 1; print usage and exit
if [ $# -lt 1 ]; then
    printf "Usage: %s <directory>\n" "$0" >&2
    exit 1
fi 

# check if compliation file exists and clean
if [[ ! -d "$1" ]]; then
    printf "ERROR : Directory '%s' does not exist\n" "$1" 
    exit 1
else 
    rm -rf "$1"/BackingStore "$1"/PARTITION
fi

exit 0