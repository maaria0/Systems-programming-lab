#!/bin/bash
files=0
dirs=0
for item in *  ; do
	if [[ -f "$item" ]]; then
		(( files++ ))
	elif [[ -d "$item" ]]; then
		(( dirs++ )) 
	fi
done
echo "Number of files: " $files
echo "Number of directories: " $dirs


	
