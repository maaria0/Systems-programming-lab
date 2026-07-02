#!/bin/bash
read -p "Enter elements : " -a arr
cnt=1
for num in "${arr[@]}" ; do
	sum=0;
	for (( i=1; i<num ;i++)) ; do
		if((num%i==0)); then
			sum=$((sum+i)) 
		fi
	done
	if((sum==num)) ; then
		if((cnt==1)); then
			echo -n $num "is Perfect"
			((cnt++))
		else 
			echo -n ", "$num "is Perfect"
		fi
	else
		if((cnt==1)); then
			echo -n $num "is Not Perfect"
			((cnt++))
		else 
			echo -n ", "$num "is Not Perfect"
		fi
	fi
done
echo
