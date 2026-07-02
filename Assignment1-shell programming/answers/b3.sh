#!/bin/bash
file=$1
count=$(cat "$1" | wc -l )
echo $count
