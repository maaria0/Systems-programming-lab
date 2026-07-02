#!/bin/bash
file=$1
if [ ! -f "$file" ] ; then
	echo "File does not exist"
	exit 1
fi
tr ' ' '\n' <"$file" | sort | uniq -c |sort -nr| awk '{print $2,$1}'

