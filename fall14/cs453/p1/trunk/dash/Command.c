#include <stdio.h>
#include <stdlib.h>
#include "Command.h"

CommandPtr createCmd(char* line)
{
	CommandPtr command;
	
	command = malloc(sizeof(Command));
	
	command->cmd = line;
	command->length = strlen(command->cmd);

	command->ORG_CMD = malloc(sizeof(char) * command->length);
	strcpy(command->ORG_CMD, command->cmd);
	
	int i;
	command->tokNum = 0;

	command->bg = FALSE;

	for(i = 0; i < command->length; i++)
	{
		if(command->cmd[i] == ' ')
		{
			command->tokNum++;
		}
		if(command->cmd[i] == '&')
		{
			command->bg = TRUE;
		}
	}
	
	command->tokNum++;

	command->tokens = malloc(sizeof(char*) * (command->tokNum + 1));
	char** start = command->tokens;
	
	*(command->tokens++) = strtok(command->cmd, " ");

	while((*(command->tokens++) = strtok(NULL, " ")));
	
	command->tokens = start;	
		
	return command;
}

Boolean isBG(CommandPtr cmd)
{
	return cmd->bg;
}

void freeCmd(CommandPtr command)
{
	free(command->tokens);
	free(command->cmd);
	free(command->ORG_CMD);
	free(command);	
}
