#!/bin/bash
file=$1
if [[ "$file" != *.c ]] ; then
	echo "Not a .c file" 
	exit 1
fi
if [ ! -f "$file" ] ; then
	echo "Not a file "
	exit 1
fi
name="${file%.c}"
gcc -E "$file" -o "$name.i"
if [ $? -eq 0 ] ; then
	echo "Successfully preprocessed "
else
	echo "Failed to preprocess"
fi
gcc -S "$file" -o "$name.s"
if [ $? -eq 0 ] ; then
	echo "Successfully generated assembly "
else 
	echo "Did not generate assembly"
fi
gcc -c "$file" -o "$name.o"
if [ $? -eq 0 ] ; then
	echo "Generated object file "
else 
	echo "Failed to generate object file"
fi
gcc "$file" -o final_exec
if [ $? -eq 0 ] ; then
	echo "Generated executable "
else 
	echo "Failed to generate executable"
fi
