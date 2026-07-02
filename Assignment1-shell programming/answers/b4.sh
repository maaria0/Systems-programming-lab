#!/bin/bash
read -p "Enter elements: " -a arr
largest=${arr[0]}
for num in "${arr[@]}" ; do
	if((num>largest)); then
		largest=$num
	fi
done
echo $largest
	
