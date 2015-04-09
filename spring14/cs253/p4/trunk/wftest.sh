#!/bin/bash
export LC_COLLATE=C

if [ $# -ne 3 ]; then
	echo "Usage: ./wftest.sh <input_directory> <expected_results> <tablesize>"
	exit 0
fi

if [ -f output.txt ]; then
	rm -f output.txt
fi

touch output.txt

cat $1*.c | ./wf $3 > output.txt

sort -k2n -k1 output.txt -o output.txt
sort -k2n -k1 $2 -o $2

diff output.txt $2

exit
