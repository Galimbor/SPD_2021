#!/bin/bash
time $'mpirun np 4' ./$1 $2'.txt' >> $2'_times.txt'
for ((c=0; c<30; c++))
do
    { time 'mpirun np 4' ./$1 $2'.txt'; } 2>&1 | cat >> $2'_times.txt'
done
