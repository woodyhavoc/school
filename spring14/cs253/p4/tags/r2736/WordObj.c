/*
 * WordObj.c
 *
 *  Created on: Apr 15, 2014
 *      Author: awoods
 */

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "WordObj.h"

WordObjPtr createWordObj(const char * wordObj, unsigned long int val)
{
	WordObjPtr wordPtr = (WordObjPtr)malloc(sizeof(WordObj));

	//wordPtr->word = malloc((sizeof(char) * strlen(wordObj)) + 1);
	wordPtr->word = (char*)wordObj;

	wordPtr->frequency = val;

	return wordPtr;
}

unsigned long int hashcode(const void * data)
{
	unsigned long int hashcode = 0;

	//WordObjPtr temp = (WordObjPtr)malloc(sizeof(WordObj));
	WordObjPtr temp = (WordObjPtr)data;

	size_t size = strlen((const char *)temp->word);

	int i;

	for(i = 0; i < size; i++)
	{
		hashcode += (temp->word[i]) * pow(31, size - (i+1));
	}

	//freeWordObj(temp);

	return hashcode;
}

char *toString(const void * data)
{
	WordObjPtr temp = (WordObjPtr)data;

	char * string = (char *)malloc(sizeof(temp->word) + 1);

	sprintf(string, "%s %lu\n", temp->word, temp->frequency);

    return string;
}

void freeWordObj(const void * data)
{
	WordObjPtr temp = (WordObjPtr)data;
	
	free((void *)temp);
}

int compareTo(const void * data1, const void * data2)
{
	WordObjPtr temp1 = (WordObjPtr)data1;
	WordObjPtr temp2 = (WordObjPtr)data2;

	return strcmp(temp1->word, temp2->word);
}
