#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sysexits.h>

/**
 * Generates 300 million numbers, and counts the number that appear
 * in the range of 90 to 110.  This is performed in an infinite loop.
 * After each iteration of the loop, the current count is printed to
 * the screen.
 */
void childProcess();

/**
 * This function is executed once the alarm period determined by
 * the user has expired. This function kills of all the currently
 * running child processes.
 *
 * @param val argument needed to make the signal function work properly,
 *            is not used within the function.
 */
void alarmHandler(int val);

/**
 * Checks the command line arguments to ensure they are correct.
 *
 * @param argc argument count
 * @param argv pointer to char array, each index is an argument
 */
void checkArgs(int argc, char * argv[]);

/**
 * Prints the proper format of the command line arguments
 * if the user enters them incorrectly.
 */
void printUsage();

pid_t* pid;
int numCPU;

/**
 * Main function.  Generates a child process for each processor
 * core.  Each child process executes the childProcess() function.
 * After an alarm goes off, and the child processes are terminated,
 * it prints the pid of each child that was terminated.  Finally,
 * the program terminates.
 */
int main(int argc, char * argv[])
{
	checkArgs(argc, argv);	
	int i;
	numCPU = sysconf(_SC_NPROCESSORS_CONF);
	
	pid = (pid_t*)malloc(sizeof(pid_t) * numCPU);

	printf("original process, pid = %u\n", getpid());
	
	for(i = 0; i < numCPU; i++)
	{
		if((pid[i] = fork()) == 0)
		{	
			printf("%dth child pid = %d\n", i, getpid());
			fflush(stdout);
			childProcess();
		}
	}
	
	signal(SIGALRM, alarmHandler);
	alarm(atoi(argv[1]));
	int status;	
	for(i = 0; i < numCPU; i++)
	{
		waitpid(pid[i], &status, 0);
		printf("pid = %d %dth child killed with signal %d (%s)\n", pid[i], i, status, strsignal(status));
	}

	free(pid);

	exit(EXIT_SUCCESS);
}

void childProcess()
{
	unsigned long i, nums = 300000000;

	unsigned int randVal, rangeCount = 0, roundCount = 0;

	srandom(getpid());
	
	while(1)
	{
		for(i = 0; i < nums; i++)
		{
			randVal = random();

			if(randVal >= 90 && randVal <= 110)
			{
				rangeCount++;
			}

		}
		
		printf("Process %u finished round %u count = %u\n", getpid(), roundCount, rangeCount);
		fflush(stdout);
		
		roundCount++;

		sleep(5);
	}

	exit(EXIT_SUCCESS);
}

void alarmHandler(int val)
{
	printf("Received alarm signal!\n");
	int i;
	for(i = 0; i < numCPU; i++)
	{
		kill(pid[i], SIGKILL);
	}
}

void checkArgs(int argc, char * argv[])
{
	if(argc != 2)
	{
		printUsage();
		exit(1);
	}
	
	int i, size = strlen(argv[1]);

	for(i = 0; i < size; i++)
	{
		if((argv[1][i] < '0' || argv[1][i] > '9') && argv[1][i] != '-')
		{
			printf("\nNon-integer value received for alarm timer: %s\n", argv[1]);
			printUsage();
			exit(1);
		}
	}

	char ** end = NULL;

	int alarmVal = (int)strtol(argv[1], end, 10);

	if(alarmVal < 0)
	{
		printf("\nInvalid alarm value given: %d\n", alarmVal);
		printUsage();
		exit(1);
	}
}

void printUsage()
{
	fprintf(stderr, "\nUsage: godot <alarm timer>  <---- Must be an integer value greater than 0\n\n");
}
