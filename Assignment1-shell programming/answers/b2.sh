#!/bin/bash
read -p "Enter elements: " -a arr
product=1
found=0
for num in "${arr[@]}" ; do
	if (( num>0 )) ; then
		product=$((product*num))
		found=1
	fi
done
if(( found==1 )) ; then
	echo $product
else 
	echo "0"
fi
