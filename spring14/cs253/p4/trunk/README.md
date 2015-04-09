*****************************************************
* Project 4: Generic Hash Table
* CS 253
* 27 Apr 2014
* Aaron Woods
*****************************************************

OVERVIEW:

This program counts the frequency of words in a file and
prints the list of words with their frequency.  This is
done using a Generic Hash Table.

INCLUDED FILES:

- common.h

	Makes a typedef of Boolean to int, used in all source files.

- doxygen-config

	Used to generate the source documentation into html files.

- Job.c

	Contains definition of a JobPtr struct, and the associated functions.
	Each NodePtr contains a JobPtr as one of it’s data members.

- Job.h

	Contains the declaration of the JobPtr struct, and the associated functions.

- List.c

	Contains definition of a ListPtr struct and the associated functions.
	The ListPtr contains NodePtrs as one of it’s data members.

- List.h

	Contains the declaration of the ListPtr struct, and the associated functions.

- Node.c

	Contains the definition of a NodePtr struct and the associated functions.
	The NodePtr contains a JobPtr as one of it’s data members, and it itself is
	used as a data member for a ListPtr.

- Node.h

	Contains the declaration of the NodePtr struct, and the associated functions.

- HashTable.c
	
	Contains definition of a HashTablePtr and the associated functions.
	The Hash Table using a Doubly Linked List in each of its indexes.

- HashTable.h

	Contains the declaration of the HashTablePtr and the associated functions.
	
- WordObj.c

	Contains definition of a WordObjPtr and the associated functions.
	A Word object is the data that is held by a NodePtr.

- WordObj.h

	Contains the declaration of the WordObjPtr and the associated functions.

- wf.c

	Word frequency program.  Reads from a file or stdin and counts the frequency
	of each word that it finds.  Prints the list after adding all words to the list.

- Makefile

	Used to compile or remove object files from the above source files.

- README (this file)

	Describes the usage of this project.

BUILDING AND RUNNING:

 Before running anything, this project must first be built.
 To do that, simply enter "make" from within the directory.

 After the project is built, the script wftest.sh can be run.
 This script runs the program, redirects the output to a file,
 sorts the file, and compares it to an expected file.

 To run the script, enter the following:

 bash wftest.sh <input directory> <expected results file> <table size>

PROGRAM DESIGN:

 The main point of using a hash table for the word frequency program
 is that it is significantly faster than using any primitive data
 structure such as an array.  Insertions and deletions can usually be
 performed in constant time.

 The program begins by checking the arguments passed.  If they pass,
 then the file or stdin is added to the hash table, one word at a time.
 This occurs in two while loops.  The first loop uses a getline, and
 the second tokenizes the line.  This continues until getline reaches
 EOF.

 After the table has been filled, the contents are printed.  Finally,
 the memory allocated to the table is freed. and the program terminates.
	 
TESTING:

 To test my program, I primarily used the sample-input directory.  I also
 used gdb to find out where my bugs were.  Between the two things, I was
 able to get my program working.

DISCUSSION:

Below are the execution times of five tests with different table sizes,
all prime numbers.

Table Size: 1

real	0m0.136s
user	0m0.008s
sys		0m0.014s

Table Size: 17

real	0m0.016s
user	0m0.007s
sys		0m0.009s

Table Size: 641

real	0m0.015s
user	0m0.007s
sys		0m0.009s

Table Size: 761

real	0m0.016s
user	0m0.007s
sys		0m0.009s

Table Size: 1009

real	0m0.016s
user	0m0.007s
sys		0m0.009s

 With a list size of 1, all the words are hashed to the same index.
 This still gives a constant insertion time, because a doubly linked
 list is being used.  However, it is much slower searching because
 it is always O(n) time since all the word objects are contained in
 the same linked list.

 The speed does not always increase as the table size increases.  The
 reason is because the linked lists at each index do not have many
 items contained in them.  Because of this, searching is still nearly
 constant because only a very small subset of the items contained in
 the table are held in any particular index.

