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

WordObjPtr createWordObj(const char *, const unsigned long int);

unsigned long int hashcode(const void *);
char *toString(const void *);
void freeWordObj(const void *);
int compareTo(const void *, const void *);

#endif /* WORDOBJ_H_ */
