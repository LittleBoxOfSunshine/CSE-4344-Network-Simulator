#!/bin/bash

echo "-----------------------------------------------"
echo "hi, $USER"
echo "How many iterations would you like to generate?"
read input
filename="input.txt"

for ((i=1;i<=input;i++)); do 
	while read -r line
	do
	    name="$line"
	    echo "Name read from file - $name"
	    ./a "$name"
	done < "$filename"	
done