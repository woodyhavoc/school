#include <common.h>
#include <Node.h>
#include <List.h>
#include "headers.h"

/**
 * Checks if the -v flag has been given when executing
 * this program.  If it has, the current svn revision
 * number is displayed, and then the program exits.
 * If it has not, the program continues execution.
 *
 * @param argc Command line argument count
 * @param argv Command line argument vector
 */
Boolean checkSvn(int argc, char** argv);

/**
 * Checks to see if the DASH_PROMPT system variable is
 * set.  If so, the user will see the contents of that
 * variable displayed as their prompt.  If not, a default
 * prompt is used instead.
 *
 * @return prompt value to be used.
 */
char* setPrompt();

/**
 * Evaluates the Command pointer, and if a background
 * process is found, it adds that process to the job list.
 * If the user enters 'exit' the program will terminate.
 *
 * @return TRUE if the user has entered 'exit', FALSE otherwise
 */
Boolean evalCmd(CommandPtr command, ListPtr jobList);

/**
 * Determines if the user has passed an empty string as
 * an argument (i.e. the just hit Return without entering
 * any text).
 *
 * @retun TRUE if the user passes an empty string, FALSE otherwise
 */
Boolean emptyString(char* line);

int main(int argc, char** argv)
{
	/*if(checkSvn(argc, argv) == TRUE)
	{
		exit(0);
	}*/
	
	ListPtr jobList = createList(compareTo, toString, freeObject);
	char* line;
	char* prompt = setPrompt();
	Boolean _exit = FALSE;

	using_history();

	while( (line=readline(prompt)) )
	{
		add_history(line);
		if(emptyString(line)) continue;
		CommandPtr command = createCmd(line);
		_exit = evalCmd(command, jobList);
		freeCmd(command);

		if(_exit)
		{
			break;
		}
	}
	freeList(jobList);
	exit(0);
}

Boolean checkSvn(int argc, char** argv)
{
	/*if(argc == 2 && strcmp(argv[1], "-v") == 0)
	{
		printf("Subversion revision number: %s\n", svn_version());
		
		return TRUE;
	}*/

	return FALSE;
}

char* setPrompt()
{
	return getenv("DASH_PROMPT") == NULL ? "myshell $ " : getenv("DASH_PROMPT");
}

Boolean evalCmd(CommandPtr command, ListPtr jobList)
{
	pid_t pid;
	int status;
	
	if(strcmp(command->tokens[0], "exit") == 0)
	{
		return TRUE;
	}

	if(strcmp(command->tokens[0], "jobs") == 0)
	{
		printList(jobList);

		return FALSE;
	}
	
	if(strcmp(command->tokens[0], "cd") == 0)
	{
		
		if(command->tokNum == 1)
		{
			struct passwd* pw = getpwuid(getuid());
			chdir(pw->pw_dir);
		}
		else
		{
			char* dir = malloc(sizeof(char) * command->length);
			int dirLen = command->length;
			int i = 3, j = 0;
			char temp[dirLen];
		
			while((temp[j++] = command->ORG_CMD[i++]));
				
			temp[j] = '\0';

			sprintf(dir, "%s", temp);

			chdir(dir);

			free(dir);
		}
		
		return FALSE;
	}

	if((pid = fork()) == 0)
	{
		if(execvp(command->tokens[0], command->tokens) < 0)
		{
			int i;
			printf("Invalid command: ");
			
			for(i = 0; i < command->tokNum; i++)
			{
				printf("%s ", command->tokens[i]);
			}
			printf("\n");
		}
		
		removeNode(jobList, createNode(createJob(getpid(), command->ORG_CMD)));
		exit(0);
	}
	else
	{	
		if(isBG(command))
		{
			JobPtr job = createJob(pid, command->ORG_CMD);
			NodePtr node = createNode(job);
			addAtRear(jobList, node);
			printf("[%u] %u %s\n", getJobCount(job), (unsigned int)pid, command->ORG_CMD);			
		}
		else
		{
			waitpid(pid, &status, 0);
		}
	}
	
	return FALSE;
}

Boolean emptyString(char* line)
{
	int i;

	for(i = 0; i < strlen(line); i++)
	{
		if(line[i] != ' ')
		{
			return FALSE;
		}
	}

	return TRUE;
}

void eofHandle(int val)
{
	printf("Received EOF signal\n");
}
