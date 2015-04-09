

#ifndef __OBJECT_H
#define __OBJECT_H

#include <stdio.h>
#include <stdlib.h>
#include "common.h"

#define MAXPID_DIGITS 20


typedef struct object  Object;
typedef struct object *ObjectPtr;

struct object {
	int key;
	char *data;
};

/**
 * Constructor function, creates an ObjectPtr and
 * returns it to the caller.
 */ 
ObjectPtr createObject (const int, const char *);

/**
 * Returns a meaningful representation of the ObjectPtr
 * to the caller.
 */
char *toString(const void *);

/**
 * Frees the space allocated to the ObjectPtr
 */
void freeObject(const void *);

/**
 * Compares two ObjectPtr, returns 1 if equal, 0 otherwise.
 */
int compareTo(const void *,const void *);


#endif /* __OBJECT_H */
