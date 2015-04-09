/*
 * HashTable.c
 *
 *  Created on: Apr 15, 2014
 *      Author: awoods
 */

#include "HashTable.h"
#include "doublyLinkedLists/Node.h"
#include "doublyLinkedLists/List.h"
#include "WordObj.h"
#include <stdio.h>

HashTablePtr createTable(int tableSize,
		unsigned long int (*hashcode)(const void *),
		char *(*toString)(const void *),
		void (*freeObject)(const void *),
		int (*compareTo)(const void *, const void *))
{
	HashTablePtr hashPtr = (HashTablePtr)malloc(sizeof(HashTable));

	hashPtr->tableSize = tableSize;
	hashPtr->numElements = 0;

	hashPtr->table = (ListPtr*)(malloc(sizeof(ListPtr) * tableSize));

	int i;

	for(i = 0; i < tableSize; i++)
	{
		hashPtr->table[i] = NULL;
	}

	hashPtr->hashcode = hashcode;
	hashPtr->toString = toString;
	hashPtr->freeObject = freeObject;
	hashPtr->compareTo = compareTo;

	return hashPtr;
}

unsigned long int hashfunc(const HashTablePtr ptable, const unsigned long int val)
{
	return val % ptable->tableSize;
}

void * searchTable(const HashTablePtr ptable, const void * data)
{
	unsigned long int hash = hashfunc(ptable, hashcode(data));

	if(ptable->table[hash] == NULL)
	{
		return NULL;
	}

	NodePtr node = search(ptable->table[hash], data);

	if(node == NULL)
	{
		return NULL;
	}

	return node->data;
}

void insert(HashTablePtr ptable, const void * data)
{
	NodePtr node = createNode((void*)data);

	unsigned long int hash = hashfunc(ptable, hashcode(data));

	if(ptable->table[hash] == NULL)
	{
		ptable->table[hash] = createList(ptable->toString, ptable->freeObject, ptable->compareTo);
	}

	addAtFront(ptable->table[hash], node);

	ptable->numElements++;
}

void * delete(HashTablePtr ptable, const void * data)
{
	unsigned long int hash = hashfunc(ptable, hashcode(data));

	NodePtr temp = ptable->table[hash]->head;

	int i, size = ptable->table[hash]->size;

	for(i = 0; i < size; i++)
	{
		if(compareTo(temp->data, data) == 0)
		{
			return removeNode(ptable->table[hash], temp);
		}

		temp = temp->next;
	}

	return NULL;
}

void freeTable(const HashTablePtr ptable)
{
	int i, size = ptable->tableSize;

	for(i = 0; i < size; i++)
	{
		if(ptable->table[i] != NULL)
		{
			freeList(ptable->table[i]);
		}
	}

	free((void *)ptable->table);
	free((void *)ptable);
}

void printTable(const HashTablePtr ptable)
{
	int i;

	for(i = 0; i < ptable->tableSize; i++)
	{
		printList(ptable->table[i]);
	}
}


