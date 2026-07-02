#!/bin/bash
read -p "Enter number : " num
gcdfnc() {
	a=$1
	b=$2
	while [ $b -ne 0 ] ; do
		r=$b
		b=$((a%b))
		a=$r
	done
	echo $a
}
result=""
for (( i=1; i< num ; i++ )) ; do
	if [ "$(gcdfnc $i $num)" -eq 1 ] ; then
		res+="$i, "
	fi
done
res=${res%, }
echo "$res"
	
		
