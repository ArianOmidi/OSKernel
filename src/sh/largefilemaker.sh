#!/bin/sh

i=0; 

while [ $i -lt $2 ]; do 
	echo "set x 100"; 
	i=$((i+1)); 
done > $1
