#!/bin/bash
read -p "Enter elements: " -a arr
largest=-1
slargest=-1
for num in "${arr[@]}" ; do
	is_prime=1
	if((num<2)) ; then
		is_prime=0;
	else
	for (( i=2; i*i<=num;i++ )) ; do
		if(( num%i==0 )) ; then
			is_prime=0
			break
		fi
	done
	fi
	if((is_prime==1)) ; then
		if ((num>largest)) ; then
			slargest=$largest
			largest=$num
		elif ((num <largest && num >slargest )) ; then
			slargest=$num
		fi
	fi
done
if((largest==-1)) ; then
	echo "No prime numbers found"
elif((slargest==-1)) ; then
	echo "No second largest prime number exists" 
else
	echo $slargest
fi

	
	
	

