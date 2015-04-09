#include <stdio.h>
#include <Windows.h>
#include <sys\types.h>

#define CHILD_PROC_TERMINATION_CODE 9
#define PROC_TERMINATION_CODE 127
#define BUFFER_SIZE 512

HANDLE hChildProc; /* child process handle */
HANDLE hTimer;
PROCESS_INFORMATION * pi;
DWORD numCPU;
SYSTEM_INFO cpuInfo;

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
 */
void CALLBACK onAlarm(PVOID lpParam, BOOLEAN timerOrWait);

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

	DWORD sec = atoi(argv[1]) * 1000, i;

	STARTUPINFO si;

	char szBuff[BUFFER_SIZE];

	GetSystemInfo(&cpuInfo);

	numCPU = cpuInfo.dwNumberOfProcessors;

	pi = (PROCESS_INFORMATION*)malloc(sizeof(PROCESS_INFORMATION)* numCPU);

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);

	if (argc == 2)
	{
		printf_s("Number of Processors: %d\n", numCPU);

		for (i = 0; i < numCPU; i++)
		{
			sprintf_s(szBuff, BUFFER_SIZE, "%s %s %d", argv[0], argv[1], i);

			if (!CreateProcess(
				NULL,		// No module name (use command line)
				szBuff,	// Command line
				NULL,		// Process handle not inheritable
				NULL,		// Thread handle not inheritable. 
				FALSE,	// Set handle inheritance to FALSE. 
				0,			// No creation flags. 
				NULL,		// Use parent's environment block. 
				NULL,		// Use parent's starting directory. 
				&si,		// Pointer to STARTUPINFO structure.
				&pi[i])	// Pointer to PROCESS_INFORMATION struct
				)
			{
				printf_s("CreateProcess failed for %s.", szBuff);
			}

			printf_s("Created %dth child, pid = %d\n", i, pi[i].dwProcessId);
		}
	}

	if (argc == 3)
	{
		childProcess();
	}

	if (argc == 2)
	{
		CreateTimerQueueTimer(&hTimer, NULL, onAlarm, NULL, sec, 0, WT_EXECUTEONLYONCE);

		DWORD exitCode;

		for (i = 0; i < numCPU; i++)
		{
			hChildProc = pi[i].hProcess;
			WaitForSingleObject(pi[i].hProcess, INFINITE);
			GetExitCodeProcess(hChildProc, &exitCode);
			printf("pid = %d %dth child killed with exit code %d\n", pi[i].dwProcessId, i, exitCode);
		}
	}

	free(pi);

	return 0;
}

void childProcess()
{
	srand(GetCurrentProcessId());

	unsigned long i, nums = 300000000;

	unsigned int randVal, rangeCount = 0, roundCount = 0;

	while (1)
	{
		for (i = 0; i < nums; i++)
		{
			randVal = rand();

			if (randVal >= 90 && randVal <= 110)
			{
				rangeCount++;
			}

		}

		printf("Process %u finished round %u count = %u\n", GetCurrentProcessId(), roundCount, rangeCount);
		fflush(stdout);

		roundCount++;

		Sleep(5);
	}

	exit(EXIT_SUCCESS);
}

/**
Callback function for the alarm. Delete the timer and terminate all child processes.
*/
void CALLBACK onAlarm(PVOID lpParam, BOOLEAN timerOrWait)
{
	DeleteTimerQueueTimer(NULL, hTimer, NULL);
	printf("Received alarm!\n");

	DWORD i;

	for (i = 0; i < numCPU; i++)
	{
		TerminateProcess(pi[i].hProcess, CHILD_PROC_TERMINATION_CODE);
	}

}

void checkArgs(int argc, char * argv[])
{
	if (argc == 3)
	{
		return;
	}

	if (argc != 2)
	{
		printUsage();
		exit(1);
	}

	int i, size = strlen(argv[1]);

	for (i = 0; i < size; i++)
	{
		if ((argv[1][i] < '0' || argv[1][i] > '9') && argv[1][i] != '-')
		{
			printf("\nNon-integer value received for alarm timer: %s\n", argv[1]);
			printUsage();
			exit(1);
		}
	}

	char ** end = NULL;

	int alarmVal = (int)strtol(argv[1], end, 10);

	if (alarmVal < 0)
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
