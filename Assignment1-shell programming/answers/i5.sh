#!/bin/bash
read -p "Enter elements of array 1: " -a arr1
read -p "Enter elements of array 2: " -a arr2
common=()
for i in "${arr1[@]}" ; do
	for j in "${arr2[@]}" ; do
		if [[ "$i" == "$j" ]]; then
			echo $i
		fi
	done
done | awk '!seen[$0]++'

