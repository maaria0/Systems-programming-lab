#!/bin/bash
for f in "$@" ; do
	if [ ! -f "$f" ] ; then
		echo "File does not exist"
		exit 1
	fi
done
objs=""
for f in "$@" ; do
	base="${f%.c}"
	obj="$base.o"
	if [ ! -f "$obj" ] || [ "$f" -nt "$obj" ] ; then
		#if object file doesnt exist or file is newer than object file
		gcc -c "$f" -o "$obj"
	fi
	objs="$objs $obj"
done
gcc $objs -o project_exec


	
	
