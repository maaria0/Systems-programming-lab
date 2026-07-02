#!/bin/bash
dir=$1 
if [[ ! -d "$dir" ]] ; then
	echo "Directory does not exist"
	exit 1
fi
size=$( du -sk "$dir" | cut -f1)
if((size > 100 )) ; then
	echo " Warning: Directory exceeds 100KB" 
else 
	echo "Size OK"
fi

