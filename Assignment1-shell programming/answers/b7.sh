#!/bin/bash
read -p "Enter string: " str
ans=$(echo "$str" | tr '[:lower:]'  '[:upper:]')
echo $ans
