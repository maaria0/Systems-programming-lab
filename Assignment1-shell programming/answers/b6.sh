#!/bin/bash
read -p "Enter string : " str
rev_string=$( echo "$str" | rev )
echo $rev_string
