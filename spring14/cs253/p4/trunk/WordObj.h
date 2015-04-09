/*
 * WordObj.h
 *
 *  Created on: Apr 2, 2014
 *      Author: marissa
 */

#ifndef WORDOBJ_H_
#define WORDOBJ_H_
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_WORD_LENGTH 50

typedef struct word WordObj;
typedef struct word *WordObjPtr;

struct word
{
	char * word;
	unsigned long int frequency;
};

/**
 * Word Object constructor
 *
 * This function creates a word object with a char * and a frequency value.
 *
 * @param word char * representing the word contained in the object
 * @param val frequency of the word appearing, initial value is 1
 *
 * @return newly created word object pointer
 */
WordObjPtr createWordObj(const char * word, const unsigned long int val);

/**
 * This function accepts data, and creates a hashcode from it.
 *
 * @param data Data to be used to generate a hashcode
 *
 * @return computed hashcode
 */
unsigned long int hashcode(const void * data);

/**
 * This function creates a string representation of the passed data
 *
 * @param data Data to be used to generate a meaningful string
 *
 * @return string representation of the passed data
 */
char *toString(const void * data);

/**
 * Frees the memory allocated to the word object
 *
 * @param data Data whose memory will be freed
 */
void freeWordObj(const void * data);

/**
 * This function compares two pieces of data and returns
 * an integer value based on the results of the comparison
 *
 * @param data1 The first data to be compared
 * @param data2 The second data to be compared
 *
 * @return negative value if data1 < data2,
 *         positive value if data1 > data2,
 *         zero if data1 == data2
 */
int compareTo(const void * data1, const void * data2);

#endif /* WORDOBJ_H_ */
