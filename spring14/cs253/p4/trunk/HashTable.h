/*
 * HashTable.h
 *
 *  Created on: Apr 2, 2014
 *      Author: marissa
 */

#ifndef HASHTABLE_H_
#define HASHTABLE_H_

#include <stdio.h>
#include <stdlib.h>
#include "doublyLinkedLists/List.h"

typedef struct hashTable HashTable;
typedef struct hashTable * HashTablePtr;

struct hashTable
{
	int tableSize;
	int numElements;
	ListPtr *table;

	unsigned long int (*hashcode)(const void *);
	char *(*toString)(const void *);
	void (*freeObject)(const void *);
	int (*compareTo)(const void *, const void *);
};

/**
 * Constructor function, creates a Hash Table pointer.
 *
 * @param tableSize The number of indexes in the table
 * @param hashcode pointer to hashcode function
 * @param toString pointer to toString function
 * @param freeObject pointer to freeObject function
 * @param compareTo pointer to compareTo function
 *
 * @return newly created hash table pointer
 */
HashTablePtr createTable(const int tableSize,
			 unsigned long int (*hashcode)(const void *),
			 char *(*toString)(const void *),
			 void (*freeObject)(const void *),
			 int (*compareTo)(const void *, const void *));

unsigned long int hashfunc(const HashTablePtr table, const unsigned long int val);

/**
 * Searches the table for the data passed as a parameter.
 *
 * @param table Hash Table to be searched
 * @param data The data to be searched for
 *
 * @return pointer to data if the search was successful,
 *         NULL otherwise
 */
void *searchTable(const HashTablePtr table, const void * data);

/**
 * Inserts data into the Hash Table.
 *
 * @param table Table the data will be inserted into
 * @param data The data to be inserted into the table
 */
void insert(HashTablePtr table, const void * data);

/**
 * Removes data from the Hash Table
 *
 * @param table Table the data will be removed from
 * @param data The data to be removed from the table
 */
void *delete(HashTablePtr table, const void * data);

/**
 * Frees the memory allocated to the Hash Table
 *
 * @param table Table whose memory will be freed
 */
void freeTable(const HashTablePtr table);

/**
 * Prints the elements of the Hash Table
 *
 * @param table Table whose elements will be printed
 */
void printTable(const HashTablePtr table);

#endif /* HASHTABLE_H_ */
