#!/bin/bash
if [ "$#" -ne 2 ] ; then
	echo "Usage:$0 <file_name> <word>"
	exit 1
fi
file=$1
word=$2
if [ ! -f "$file" ] ; then
	echo "File does not exist " 
	exit 1
fi
lines=$(grep -n -w "$word" "$file" | cut -d: -f1)
words=$(grep -o -w "$word" "$file" | wc -l )
if [ -z "$lines" ] ; then
	echo "No such word exists"
	exit 1
fi
echo "Word found at line numbers :" $lines
echo "Total number of occurrences :" $words

