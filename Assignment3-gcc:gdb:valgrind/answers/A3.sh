#!/bin/bash
file=$1
flag=$2
if [ ! -f "$file" ] ; then
	echo "Not a file"
	exit 1
fi
if [[ "$file" != *.c ]] ; then
	echo "Not a c file "
	exit 1
fi
if [ "$flag" = "-E" ] ; then
	#preprocessing
	gcc -E "$file" -o "${file%.c}.i"
fi
if [ "$flag" = "-S" ] ; then
	gcc -S "$file" -o "${file%.c}.s"
fi
if [ "$flag" = "-c" ] ; then
	gcc -c "$file" -o "${file%.c}.o"
fi
