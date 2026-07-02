#!/bin/bash
read -p "Enter integer: " num
for (( i=2 ; i<=num ; i++ )) ; do
	is_prime=1;
	for ((j=2 ; j*j<=i ; j++)); do
		if(( i % j==0 )) then
		is_prime=0;
		fi
	done
	if((is_prime==1)) ; then
		printf "%s%d" "$sep" "$i"
		sep=", "
	fi
done
echo

	
		
	
