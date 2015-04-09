/**
 * Author: Aaron Woods
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sysexits.h>
#include "FileAccess.h"

/**
 * Main function, responsible for generating the threads
 * that will read the file, and then joining them back together
 * after the read operations have been completed.
 */
int main()
{
	int i;

	// Number of processors on the system.
	int nproc = sysconf(_SC_NPROCESSORS_CONF);

	// Makes sure the case where the thread number is too high occurs.
	int threadRead = nproc * 2;
	
	// Initializes the static variables in the FileAccess class.
	FileAccess(nproc);

	pthread_t* threads = (pthread_t*)malloc(sizeof(pthread_t) * threadRead);

	int* threadIDs = (int*)malloc(sizeof(int) * threadRead);

	// Create more threads than number of processors
	for(i = 0; i < threadRead; i++)
	{
		threadIDs[i] = i+1;
		pthread_create(&threads[i], NULL, startAccess, &threadIDs[i]);
	}

	for(i = 0; i < threadRead; i++)
	{
		pthread_join(threads[i], NULL);
	}
	
	free(threads);
	free(threadIDs);
	
	return 0;
}
