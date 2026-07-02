#!/bin/bash
read -p "Enter a string: " str
rev_str=$(echo "$str" | rev )
if [ "$rev_str" == "$str" ] ; then
	echo "Palindrome"
else 
	echo "Not a palindrome" 
fi
