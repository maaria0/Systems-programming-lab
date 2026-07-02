#!/bin/bash
gcc -fPIC -c arith.c -o arith.o
gcc -shared arith.o -o libarith.so
gcc main.c -L. -larith -o main
LD_LIBRARY_PATH=. ./main
