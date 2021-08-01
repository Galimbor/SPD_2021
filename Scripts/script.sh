#!/bin/bash
./$1 $2'.txt'
for ((c=0; c<30; c++))
do
    { time ./$1 $2'.txt'; } 2>&1 | cat >> $3
done
