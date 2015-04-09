#ifndef __COMMAND_H
#define __COMMAND_H

#include <stdio.h>
#include <stdlib.h>
#include <common.h>

typedef struct command Command;
typedef struct command* CommandPtr;

struct command
{
	unsigned int length;
	unsigned int tokNum;
	char** tokens;
	char* cmd;
	char* ORG_CMD;
	Boolean bg;
};

/**
 * Creates a Command pointer which is used
 * by the dash in order to execute commands.
 * The function accepts a char* which is the
 * command, then parses it into token strings
 * which will later be passed to execvp.
 *
 * @param line char* containing a command, to be 
 * 					 tokenized
 */
CommandPtr createCmd(char* line);

/**
 * Returns the Command variable bg.
 * bg is TRUE if the command has a '&'
 * as the last character
 *
 * @return TRUE if the command will be run in
 * 		   the background, FALSE otherwise
 */
Boolean isBG();

/**
 * Frees the Command pointer's char* cmd, followed
 * by freeing the pointer itself.
 *
 * @param cmd the Command pointer to be freed
 */
void freeCmd(CommandPtr cmd);


#endif /* __COMMAND_H */
