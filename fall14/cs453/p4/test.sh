#! /bin/bash
echo "" > booga.log
echo "" > booga.tmp

make

# Unload the driver initially to ensure that /proc/driver/booga is starting fresh
./booga_unload

# Load the driver
./booga_load
echo -ne "Loaded booga driver to kernel space.\n"

declare -a MIN_NUM

COUNTER=0
READ=0
WRITE=0
MIN_NUM=(0 0 0 0)
RAND_BYTES=0
RAND_MIN=0
TESTNUM=$RANDOM
BOOGA=0
GOOGOO=0
NEKA=0
WOOGA=0
LINECOUNT=0

echo -ne "Performing $TESTNUM read tests on booga!\n"

while [ $COUNTER -le 5 ]; do
	
	RAND_BYTES=$RANDOM
	RAND_MIN=$RANDOM
	let RAND_MIN=RAND_MIN%4
	let READ=READ+RAND_BYTES
	let MIN_NUM[$RAND_MIN]=MIN_NUM[$RAND_MIN]+1

	./test-booga $RAND_MIN $RAND_BYTES read > test.tmp
	
	tail -n +2 test.tmp > message.tmp

	if grep -q 'b' message.tmp; 
	then
		let BOOGA=BOOGA+1
	elif grep -q 'w' message.tmp;
	then			
		let WOOGA=WOOGA+1
	elif grep -q 'n' message.tmp;
	then
		let NEKA=NEKA+1
	elif grep -q 'g' message.tmp;
	then
		let GOOGOO=GOOGOO+1
	fi

	cat test.tmp >> booga.log
	let COUNTER=COUNTER+1
done

rm -f message.tmp
rm -f test.tmp

let COUNTER=0

echo -ne "Performing $TESTNUM write tests on booga!\n"

while [ $COUNTER -le 5 ]; do
	
	RAND_BYTES=$RANDOM
	RAND_MIN=$RANDOM
	let RAND_MIN=RAND_MIN%3
	let WRITE=WRITE+RAND_BYTES
	let MIN_NUM[$RAND_MIN]=MIN_NUM[$RAND_MIN]+1

	./test-booga $RAND_MIN $RAND_BYTES write >> booga.log

	let COUNTER=COUNTER+1
done

echo -ne "Writing to minor number 3, test should terminate.\n"

let MIN_NUM[3]=MIN_NUM[3]+1

let WRITE=WRITE+100

./test-booga 3 100 write

printf "bytes read = %ld\n" "$READ" > booga.stats
printf "bytes written = %ld\n" "$WRITE" >> booga.stats
printf "number of opens:\n" >> booga.stats
printf "\t/dev/booga0 = %ld times\n" "${MIN_NUM[0]}" >> booga.stats
printf "\t/dev/booga1 = %ld times\n" "${MIN_NUM[1]}" >> booga.stats
printf "\t/dev/booga2 = %ld times\n" "${MIN_NUM[2]}" >> booga.stats
printf "\t/dev/booga3 = %ld times\n" "${MIN_NUM[3]}" >> booga.stats
printf "strings output:\n" >> booga.stats
printf "\tbooga! booga! = %ld times\n" "$BOOGA" >> booga.stats
printf "\tgoogoo! gaga! = %ld times\n" "$GOOGOO" >> booga.stats
printf "\tneka! maka! = %ld times\n" "$NEKA" >> booga.stats
printf "\twooga! wooga! = %ld times\n" "$WOOGA" >> booga.stats


echo -ne "Expected Results:\n\n"
cat booga.stats

echo -ne "Actual Results:\n\n"
cat /proc/driver/booga

echo -ne "Diff Results:\n\n"
diff booga.stats /proc/driver/booga

# Unload the driver
./booga_unload
echo -ne "Unloaded booga driver from kernel space.\n"

make clean
