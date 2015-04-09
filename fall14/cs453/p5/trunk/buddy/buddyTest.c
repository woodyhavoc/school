#include <stdio.h>
#include <stdlib.h>
#include "buddy.h"

int main(int argc, char** argv)
{
	//buddy_init(0);

	printBuddyLists();
	
	int i;
	unsigned long size = atol(argv[1]);
	//int ** ptr = (int**) buddy_malloc(sizeof(int*) * size);
	
	int *num = (int*) buddy_malloc(sizeof(int) * size);

	printBuddyLists();
	
	/*
	for(i = 0; i < size; i++)
	{
		ptr[i] = (int*) buddy_malloc(sizeof(int) * i);
		*ptr[i] = i * i;
		printf("Value at index %d: %d\n", i, *ptr[i]);
	}


	for(i = 0; i <= size; i++)
	{
		printf("Freeing index %d.\n", i);
		buddy_free(ptr[i]);
	}
	*/
	printf("Freeing top level pointer.\n");
	buddy_free(num);

	//printBuddyLists();

	//buddy_free(temp);
	return 0;
}
