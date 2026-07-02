#!/bin/bash
if [ "$#" -ne 4 ] ; then
	echo "Usage:$0 <input_file> <old_word> <new_word> <output_file>"
fi
input_file=$1 
old_word=$2
new_word=$3
output_file=$4
if [ ! -f "$input_file" ] ; then
	echo "No such file exists"
	exit 1
fi
sed "s/${old_word}/${new_word}/g" "$input_file">"$output_file"



