#!/bin/bash

echo "-----------------------------------------------"
echo "hi, $USER"
echo "How many iterations would you like to generate?"
read input
filename="input.txt"

settings = "$1Settings"
message = "$1Messages"
output = "$1Log"
low = $2
high = $3

# for ((i=1;i<=input;i++)); do 
# 	while read -r line
# 	do
# 	    name="$line"
# 	    echo "Name read from file - $name"
# 	    ./a.out "$name"
# 	done < "$filename"	
# done

for ((i=$low;i<=$high;i++)); do
	./a.out "$settings$i" "$message$i" "$output$i"
done