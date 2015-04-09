#!/bin/sh

echo ""

if [ $# -eq 0 ]
	then
for((i=0; i<100; i++))
	do
		let "ARG1 = 100 * $i"
		let "ARG2 = 100 * $(($i+1))"
		let "SEED = $RANDOM * $RANDOM"
		echo "Running test $(($i+1)) with list size = $ARG1, num. operations = $ARG2, seed value = $SEED"
		valgrind --leak-check=yes ./RandomTestList $ARG1 $ARG2 $SEED
		if [ $? -eq 139 ]; then
			    echo -e "\n\n!!!!! SEGMENTATION FAULT ON TEST $i, list size = $ARG1, num.operations = $ARG2, seed value = $SEED !!!!!\n\n"
				    exit 1
		fi		
		echo ""

	done
	exit
fi

if [ $# -eq 1 ]
then
for((i=0; i<"$1"; i++))
	do
		let "ARG1 = $RANDOM * $i"
		let "ARG2 = $RANDOM * $(($i+1))"
		let "SEED = $RANDOM * $RANDOM"
		echo "Running test $(($i+1)) with list size = $ARG1, num. operations = $ARG2, seed value = $SEED"
		valgrind --leak-check=yes ./RandomTestList $ARG1 $ARG2 $SEED
		if [ $? -eq 139 ]; then
			    echo -e "\n\n!!!!! SEGMENTATION FAULT ON TEST $i, list size = $ARG1, num.operations = $ARG2, seed value = $SEED !!!!!\n\n"
				    exit 1
		fi		
		echo ""

	done
exit
fi
