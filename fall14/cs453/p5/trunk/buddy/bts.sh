#! /bin/bash

make

gcc -g -c buddyTest.c
gcc -g -o buddyTest buddyTest.o buddy.o -lm

COUNT=0

while [ $COUNT -le 36 ]; do
	./buddyTest $((2**$COUNT))
	let COUNT=COUNT+1
	sleep 1
done

LD_LIBRARY_PATH=$(pwd) LD_PRELOAD=libbuddy.so ./buddy-test 10 1234 verbose

make clean
