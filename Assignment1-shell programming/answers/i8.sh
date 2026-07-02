a=$1
b=$2
c=$3
if (( a+b > c && b+c > a && c+a > b )) ; then
	if (( a*a + b*b == c*c || b*b + c*c == a*a || a*a + c*c == b*b )) ; then
		echo "It is a Triangle and follows Pythagoras property"
	else
		echo "It is a Triangle but does not follow Pythagoras property"
	fi
else
	echo "Not a triangle" 
fi

