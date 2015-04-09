/**
 * Author: Aaron Woods
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include "FileAccess.h"

//private variables
static int maxThreads;
static int threadsTotal;

static pthread_mutex_t fileMutex;
static pthread_cond_t fileCond;

void FileAccess(int n)
{
	maxThreads = n - 1;
	threadsTotal = 0;
	pthread_mutex_init(&fileMutex, NULL);
	pthread_cond_init(&fileCond, NULL);
}

void* startAccess(void* obj)
{
	pthread_mutex_lock(&fileMutex);

	int threadID = *(int*) obj;

	if(threadID > maxThreads)
	{
		printf("File Access Denied to Thread ID: %2d, ID number is too large.\n", threadID);
		fflush(stdout);
		pthread_mutex_unlock(&fileMutex);
		return (void*) 0;
	}

	while((threadsTotal + threadID) > maxThreads)
	{
		pthread_cond_wait(&fileCond, &fileMutex);
	}
	
	threadsTotal += threadID;

	pthread_mutex_unlock(&fileMutex);

	printf("Thread ID: %2d starting file access. Sum of thread IDs: %2d.\n", threadID, threadsTotal);
	fflush(stdout);
	
	endAccess(*(int*)obj);
	
	return (void*) 0;
}

void endAccess(int threadID)
{
	pthread_mutex_lock(&fileMutex);

	threadsTotal -= threadID;
	
	printf("Thread ID: %2d ending file access. Sum of thread IDs: %2d.\n", threadID, threadsTotal);
	fflush(stdout);

	pthread_mutex_unlock(&fileMutex);

	pthread_cond_signal(&fileCond);

	//pthread_exit(0);
}
