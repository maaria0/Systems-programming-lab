#!/bin/bash
file=$1
log=compile.log
>$log
gcc -E "$file" -o program.i 2>>$log
gcc -S "$file" -o program.s 2>>$log
gcc -c "$file" -o program.o 2>>$log
gcc program.o -o program 2>>$log
warnings=$(grep -c "warning:" $log)
errors=$(grep -c "error:" $log)
echo "Warnings:"$warnings""
echo "Errors:"$errors""
