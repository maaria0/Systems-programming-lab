#!/bin/bash
read -p "Enter elements: " -a arr
count=0;
for num in "${arr[@]}" ; do
	if(( num > 0 )) ; then
		count=$((count + 1 ))
	fi
done
echo $count
