/**
 * Author: Aaron Woods
 */

#ifndef __BUDDY_H
#define __BUDDY_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

typedef struct header Header;
typedef struct header* HeaderPtr;

struct header
{
	char TAG;
	char KVAL;
	HeaderPtr LINKF;
	HeaderPtr LINKB;
};

/**
 * Creates a header pointer at the address given
 * by memPool, with a exponent value given by power.
 *
 * @param memPool address at which the HeaderPtr will reside
 * @param power header exponent
 */
HeaderPtr createHeader(void* memPool, int power);

/**
 * Initializes a block of memory with the number
 * of bytes that is the result of 2^size
 *
 * @param size the exponent to compute the number of bytes
 * 			   to allocate
 */
void buddy_init(size_t size);

/**
 * Reserves a block of memory and zeroes out the data
 * that is written within the bounds.
 *
 * @param size1 the number of elements that will be stored
 * 				in the memory
 * @param size2 the number of bytes each element occupies
 */
void* buddy_calloc(size_t size1, size_t size2);

/**
 * Allocates a block of memory with the size given
 * as 2^size
 *
 * @param size the exponent used to calculate the
 * 			   amount of memory to allocate.
 */
void* buddy_malloc(size_t size);

/**
 * Increases the size of an existing block of memory.
 *
 * @param ptr the pointer to the original block of memory
 * @param size the size by which the block of memory will
 * 			   be increases (2^size bytes)
 */
void* buddy_realloc(void* ptr, size_t size);

/**
 * Returns a block of memory to the system
 *
 * @param ptr the block of memory to be returned
 * 		      to the system
 */
void buddy_free(void* ptr);

/**
 * Prints the contents of the memory lists
 */
void printBuddyLists();

/**
 * Overrides the system malloc function, and calls
 * buddy_malloc
 *
 * @param size the size of the memory block to be allocated
 * 			   (2^size bytes)
 */
void* malloc(size_t size);

/**
 * Overrides the system free function and calls buddy_free
 *
 * @param ptr the pointer to the memory which will be freed
 */
void free(void* ptr);

/**
 * Overrides the system calloc function and calls buddy_calloc
 *
 * @param nmemb the number of elements to be stored in the memory
 * @param size the number of bytes each element occupies
 */
void* calloc(size_t nmemb, size_t size);

/**
 * Overrides the system realloc function and calls buddy_realloc
 *
 * @param ptr the pointer to the original block of memory to be
 * 			  resized
 * @param size the size of the new block of memory to be allocated
 * 			   (2^size bytes)
 */
void* realloc(void* ptr, size_t size);

#endif /* __BUDDY_H */
