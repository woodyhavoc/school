/**
 * Author: Aaron Woods
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "buddy.h"

static void computeMinSize();
static void initHeaders();
static int findK(size_t size);

typedef unsigned int Boolean;

#define TRUE 1
#define FALSE 0

#define FREE 0
#define RESERVED 1

#define MAX_SIZE 36
#define DEFAULT_SIZE 29
#define DEFAULT_KVAL 29
#define CUTOFF 31

HeaderPtr headers[MAX_SIZE];

static Boolean initialized = FALSE;
static unsigned int HEADER_SIZE;
static unsigned int MIN_SIZE;
static unsigned int TAG_SIZE;
static unsigned int KVAL_SIZE;
static void* BASE_ADDR;
static unsigned long TOTAL_MEMORY;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

HeaderPtr createHeader(void* memPool, int power)
{
	if(memPool == NULL)
	{
		return NULL;
	}

	HeaderPtr header = (HeaderPtr) memPool;

	header->TAG = FREE;
	header->KVAL = (char) power;
	header->LINKF = NULL;
	header->LINKB = NULL;

	return header;
}

void buddy_init(size_t size)
{
	BASE_ADDR = sbrk(0);
	TOTAL_MEMORY = 1ULL << (MAX_SIZE - 1);
	computeMinSize();

	printf("Minimum power of 2: %d\n", MIN_SIZE);

	initHeaders();
	void* memPool;
	unsigned long memAlloc;

	if(size == 0)
	{
		memAlloc = 1 << DEFAULT_SIZE;
		memPool = sbrk(memAlloc);
		headers[DEFAULT_SIZE] = createHeader(memPool, DEFAULT_SIZE);
		TAG_SIZE = sizeof(headers[DEFAULT_SIZE]->TAG);
		KVAL_SIZE = sizeof(headers[DEFAULT_SIZE]->KVAL);
		initialized = TRUE;
		TOTAL_MEMORY -= memAlloc;
		return;
	}
	
	int k = findK(size);
	
	if(k >= MAX_SIZE)
	{
		printf("Memory allocation request too high. Exiting.\n");
		exit(1);
	}
	else if(k > CUTOFF)
	{
		memAlloc = 1 << CUTOFF;
		memPool = NULL;
		int i;
		unsigned int diff = k - CUTOFF;
		
		for(i = 0; i < pow(2, diff); i++)
		{
			if(i == 0)
			{
				memPool = sbrk(memAlloc);
				continue;
			}
			sbrk(memAlloc);
		}
		
		memAlloc *= pow(2, diff);

		headers[k] = createHeader(memPool, k);
		TOTAL_MEMORY -= memAlloc;
		TAG_SIZE = sizeof(headers[k]->TAG);
		KVAL_SIZE = sizeof(headers[k]->KVAL);
	}
	else if(k < MIN_SIZE)
	{
		memAlloc = 1 << MIN_SIZE;
		memPool = sbrk(memAlloc);
		headers[MIN_SIZE] = createHeader(memPool, MIN_SIZE);
		TOTAL_MEMORY -= memAlloc;
		TAG_SIZE = sizeof(headers[MIN_SIZE]->TAG);
		KVAL_SIZE = sizeof(headers[MIN_SIZE]->KVAL);
		memAlloc = 1ULL << MIN_SIZE;
	}
	else
	{
		memAlloc = 1UL << k;
		memPool = sbrk(memAlloc);
		headers[k] = createHeader(memPool, k);
		TOTAL_MEMORY -= memAlloc;
		TAG_SIZE = sizeof(headers[k]->TAG);
		KVAL_SIZE = sizeof(headers[k]->KVAL);
	}

	initialized = TRUE;
}

void* buddy_calloc(size_t size1, size_t size2)
{
	if(size1 == 0 || size2 == 0)
	{
		return NULL;
	}

	if(!initialized)
	{
		buddy_init(size1 * size2);
	}

	unsigned long size = size1 * size2, i;

	void* calMem = buddy_malloc(size);

	char* zero = (char*) calMem;

	for(i = 0; i < size; i++)
	{
		*zero = 0;
		zero++;
	}

	return (void*) calMem;
}

void* buddy_malloc(size_t size)
{
	if(!initialized)
	{
		buddy_init(size);
	}
	
	int k = findK(size);

	if(k >= MAX_SIZE || TOTAL_MEMORY - (1UL << k) < 0)
	{
		printf("Asking for too much memory fool.\n");
		exit(1);
	}

	int startK = k;

	while(headers[k] == NULL)
	{
		k++;
	}

	if(k == startK)
	{
		int kval = (int)headers[k]->KVAL;
		unsigned long memLoc = (unsigned long)headers[k];
		memLoc -= (unsigned long) BASE_ADDR;
		unsigned long buddyMemLoc = memLoc | (1UL << (kval - 1));
		memLoc += (unsigned long) BASE_ADDR;
		buddyMemLoc += (unsigned long) BASE_ADDR;
		
		if(headers[k]->LINKF == NULL)
		{
			HeaderPtr pal = createHeader((void*)memLoc, kval-1);
			HeaderPtr buddy = createHeader((void*)buddyMemLoc, kval-1);
			
			pal->LINKF = buddy;
			buddy->LINKB = pal;

			if(headers[k-1])
			{
				buddy->LINKF = headers[k-1]->LINKF;

				if(buddy->LINKF != NULL)
				{
					buddy->LINKF->LINKB = buddy;
				}
			}			

			headers[k-1] = pal;	
			headers[k] = NULL;
		}
		else
		{
			memLoc = (unsigned long)headers[k]->LINKF;
			memLoc -= (unsigned long) BASE_ADDR;
			buddyMemLoc = memLoc | (1UL << (kval - 1));
			memLoc += (unsigned long) BASE_ADDR;
			buddyMemLoc += (unsigned long) BASE_ADDR;

			HeaderPtr pal = createHeader((void*)memLoc, kval-1);
			HeaderPtr buddy = createHeader((void*)buddyMemLoc, kval-1);
			
			pal->LINKF = buddy;
			buddy->LINKB = pal;
			
			if(headers[k-1])
			{
				buddy->LINKF = headers[k-1]->LINKF;

				if(buddy->LINKF)
				{
					buddy->LINKF->LINKB = buddy;
				}
			}

			headers[k-1] = pal;
			headers[k]->LINKF = NULL;

			TOTAL_MEMORY -= (1UL << (kval-1));
		}

		k--;

		printBuddyLists();
	
		HeaderPtr ptr = headers[k];
		headers[k] = ptr->LINKF;

		if(headers[k])
		{
			headers[k]->LINKB = NULL;
		}

		ptr->LINKF = NULL;
		ptr->TAG = RESERVED;
		ptr += TAG_SIZE + KVAL_SIZE;

		return  (void*) ptr;
	}

	while(k > startK)
	{
		int kval = (int)headers[k]->KVAL;
		unsigned long memLoc = (unsigned long)headers[k];
		memLoc -= (unsigned long) BASE_ADDR;
		unsigned long buddyMemLoc = memLoc | (1UL << (kval - 1));
		memLoc += (unsigned long) BASE_ADDR;
		buddyMemLoc += (unsigned long) BASE_ADDR;
		
		if(headers[k]->LINKF == NULL)
		{
			HeaderPtr pal = createHeader((void*)memLoc, kval-1);
			HeaderPtr buddy = createHeader((void*)buddyMemLoc, kval-1);
			
			pal->LINKF = buddy;
			buddy->LINKB = pal;

			if(headers[k-1])
			{
				buddy->LINKF = headers[k-1]->LINKF;

				if(buddy->LINKF)
				{
					buddy->LINKF->LINKB = buddy;
				}
			}			

			headers[k-1] = pal;	
			headers[k] = NULL;

			TOTAL_MEMORY -= (1UL << (k-1));
		}
		else
		{
			memLoc = (unsigned long)headers[k]->LINKF;
			memLoc -= (unsigned long) BASE_ADDR;
			buddyMemLoc = memLoc | (1UL << (kval - 1));
			memLoc += (unsigned long) BASE_ADDR;
			buddyMemLoc += (unsigned long) BASE_ADDR;

			HeaderPtr pal = createHeader((void*)memLoc, kval-1);
			HeaderPtr buddy = createHeader((void*)buddyMemLoc, kval-1);
			
			pal->LINKF = buddy;
			buddy->LINKB = pal;
			
			if(headers[k-1])
			{
				buddy->LINKF = headers[k-1]->LINKF;

				if(buddy->LINKF)
				{
					buddy->LINKF->LINKB = buddy;
				}
			}

			headers[k-1] = pal;
			headers[k]->LINKF = NULL;

			TOTAL_MEMORY -= (1UL << (k-1));
		}

		k--;
	}

	HeaderPtr ptr = headers[k];
	headers[k] = ptr->LINKF;
	
	if(headers[k])
	{
		headers[k]->LINKB = NULL;
	}

	ptr->LINKF = NULL;
	ptr->TAG = RESERVED;
	ptr+= TAG_SIZE + KVAL_SIZE;

	return (void*) ptr;	
}

void* buddy_realloc(void* ptr, size_t size)
{
	if(ptr == NULL)
	{
		return NULL;
	}

	void* copy = buddy_malloc(size);

	unsigned long cpySize = (unsigned long) (1UL << (((HeaderPtr)ptr)->KVAL));

	memcpy(copy, ptr, cpySize);

	return (void*) copy;
}

void buddy_free(void* ptr)
{
	if(ptr == NULL)
	{
		return;
	}
	
	HeaderPtr pal = (HeaderPtr) (ptr - (TAG_SIZE + KVAL_SIZE));	
	
	int kval = pal->KVAL;
	
	pal->LINKF = headers[kval]->LINKF;
	pal->LINKB = NULL;
	pal->TAG = FREE;
	headers[kval]->LINKF->LINKB = pal;
	headers[kval] = pal;

	kval++;
	
	while(headers[kval]->LINKF != NULL)
	{
		 headers[kval]->LINKF = NULL;

		 headers[kval]->LINKF = headers[kval+1]->LINKF;
		 headers[kval+1]->LINKF->LINKB = headers[kval];
		 headers[kval+1] = headers[kval];
		 headers[kval] = NULL;
		 kval++;
	}

	pal->LINKF = headers[kval]->LINKF;
	pal->LINKB = NULL;
	pal->TAG = FREE;
	headers[kval]->LINKF->LINKB = pal;

	headers[kval] = pal;

	TOTAL_MEMORY += (1UL << kval);
}

void printBuddyLists()
{
	int i;

	for(i = 0; i < MAX_SIZE; i++)
	{
		printf("List %2d: head = %p --> ", i, &headers[i]);

		if(headers[i] == NULL)
		{
			printf("<null>\n");
		}
		else
		{
			if(headers[i]->TAG == FREE)
			{
				printf("[tag=FREE, ");
			}
			else
			{
				printf("[tag=RESERVED, ");
			}
			
			printf("kval=%2d, addr=%10p] --> ", headers[i]->KVAL, headers[i]);	
			
			if(headers[i]->LINKF == NULL)
			{
				printf("<null>\n");
			}
			else
			{
				printf("%p\n", headers[i]->LINKF);
			}
		}
	}
}

/**
 * Computes the minimum k value based on
 * the size of the header
 */
static void computeMinSize()
{
	HEADER_SIZE = sizeof(Header);
	
	int k = 1;

	while((1 << k) < HEADER_SIZE)
	{
		k++;
	}

	MIN_SIZE = k;
}

/**
 * Initializes all the HeaderPtr indices to NULL
 */
static void initHeaders()
{
	int i;

	for(i = 0; i < MAX_SIZE; i++)
	{
		headers[i] = NULL;
	}
}

/**
 * Finds the best-fit value of k based on the size
 * given in the parameter
 *
 * @param size value used to find the best-fit value of k
 * 
 * @return the best-fit value of k
 */
static int findK(size_t size)
{
	int k = 1;

	unsigned long top = size + HEADER_SIZE;

	while((1UL << k) < top)
	{
		k++;
	}

	return k > MIN_SIZE ? k : MIN_SIZE;
}

void* malloc(size_t size)
{
	pthread_mutex_lock(&mutex);

	void* ret = buddy_malloc(size);

	pthread_mutex_unlock(&mutex);

	return ret;
}

void free(void* ptr)
{
	pthread_mutex_lock(&mutex);

	buddy_free(ptr);

	pthread_mutex_unlock(&mutex);
}

void* calloc(size_t nmemb, size_t size)
{
	pthread_mutex_lock(&mutex);

	void* ret = buddy_calloc(nmemb, size);

	pthread_mutex_unlock(&mutex);

	return ret;
}

void* realloc(void* ptr, size_t size)
{
	pthread_mutex_lock(&mutex);

	void* ret = buddy_realloc(ptr, size);

	pthread_mutex_unlock(&mutex);

	return ret;
}
