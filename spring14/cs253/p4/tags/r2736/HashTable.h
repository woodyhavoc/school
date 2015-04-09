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

HashTablePtr createTable(const int tableSize,
			 unsigned long int (*hashcode)(const void *),
			 char *(*toString)(const void *),
			 void (*freeObject)(const void *),
			 int (*compareTo)(const void *, const void *));

unsigned long int hashfunc(const HashTablePtr table, const unsigned long int);

void *searchTable(const HashTablePtr, const void *);
void insert(HashTablePtr, const void *);
void *delete(HashTablePtr, const void *);

void freeTable(const HashTablePtr);
void printTable(const HashTablePtr);

#endif /* HASHTABLE_H_ */
