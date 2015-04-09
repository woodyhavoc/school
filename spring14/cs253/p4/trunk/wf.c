/*
 * wf.c
 *
 *  Created on: Apr 15, 2014
 *      Author: awoods
 */

#include <string.h>
#include <stdio.h>

#include "doublyLinkedLists/List.h"
#include "doublyLinkedLists/Node.h"
#include "doublyLinkedLists/common.h"
#include "HashTable.h"
#include "WordObj.h"

/**
 * This function checks the command line arguments passed
 * to the program.  If there are any pretenders among the
 * group, they are all dealt with, Heisenberg style.  To
 * survive, the number of arguments must be either 2 or 3.
 * Also, the first argument (after the program name), must
 * be a long value representing the size of the table.  The
 * next argument, if there is one, specifies the file to 
 * search.
 *
 * @param argc The first argument that claims to be an integer
 * 			   value representing the number of command line
 * 			   arguments.  Heisenberg is going to thoroughly
 * 			   test this one to see if it has what it takes.
 *
 * @param argv The second argument, it says it holds all of the
 * 			   arguments in an array.  Heisenberg will determine
 * 			   if its story checks out.
 */
void breakingBadArgs(int argc, char *argv[]);

/**
 * This function is a eulogy for those command-line
 * arguments that were not clever enough to fool Heisenberg.
 *
 * It gives a warning to future arguments, telling them what
 * they need to do in order to survive Heisenberg.
 */
void printUsage();

/**
 * Fills the hash table using a file pointer which reads
 * the file one line at a time.
 *
 * @param table Hash Table that will be filled with information
 *              from the file.
 *
 * @param argv This is used so the file name can be extracted from
 *             the command line arguments.
 */
void fillTableFile(HashTablePtr table, char * argv[]);

/**
 * Fills the hash table using stdin, which is either a pipe
 * from the cat command, or a redirection from a file.
 *
 * @param table Hash Table that will be filled with information
 *              from the file.
 */
void fillTableSTD(HashTablePtr table);

const char* delimiters = " \t;{}()[].#<>\n\r+-/%*\"^'~&=!|:\\?,";

/**
 * Main function of the Word Frequency Program.  This function
 * drives the reading of a file(s) to count the frequency
 * of the words contained within.  After it has done this, it
 * prints the results.  Finally it frees the memory used by
 * the table and returns control to the operating system.
 */
int main(int argc, char *argv[])
{
    breakingBadArgs(argc, argv);

	HashTablePtr table = createTable(atol(argv[1]), &hashcode, &toString, &freeWordObj, &compareTo);

	if(argc == 2)
	{
		fillTableSTD(table);
	}
	else
	{
		fillTableFile(table, argv);
	}

	printTable(table);
	freeTable(table);

	return 0;
}

void breakingBadArgs(int argc, char *argv[])
{
	if(argc != 2 && argc != 3)
	{
		printUsage();
		exit(1);
	}
}

void printUsage()
{
	printf("\n\n");
	printf("****************** USAGE *******************\n");
	printf("*\n");
	printf("* <program executable> <file to be searched>\n");
	printf("*\n");
	printf("****************** USAGE *******************\n");
	printf("\n\n");
}

void fillTableFile(HashTablePtr table, char * argv[])
{
	FILE *file;

	file = fopen(argv[2], "r");

	size_t size = 0;

	char *istring = NULL;

	char *tstring = NULL;

	while(getline(&istring, &size, file) != EOF)
	{
		tstring = strtok(istring, delimiters);

		while(tstring != NULL)
		{
			WordObjPtr data = createWordObj(tstring, 1);
			WordObjPtr result = (WordObjPtr)searchTable(table, data);

			if(result != NULL)
			{
				result->frequency++;
				table->numElements++;
				freeWordObj(data);
			}
			else
			{
				insert(table, data);
			}
			
			tstring = strtok(NULL, delimiters);
		}

		istring = NULL;
		free(istring);
		size = 0;
		//char * istring = NULL;
	}
	
	free(istring);

	fclose(file);
}

void fillTableSTD(HashTablePtr table)
{
	size_t size = 0;

	char *istring = NULL;

	char *tstring = NULL;

	while(getline(&istring, &size, stdin) != EOF)
	{
		tstring = strtok(istring, delimiters);

		while(tstring != NULL)
		{
			WordObjPtr data = createWordObj(tstring, 1);
			WordObjPtr result = (WordObjPtr)searchTable(table, data);

			if(result != NULL)
			{
				result->frequency++;
				table->numElements++;
				freeWordObj(data);
			}
			else
			{
				insert(table, data);
			}

			tstring = strtok(NULL, delimiters);
		}

		istring = NULL;
		free(istring);
		size = 0;
	}

	free(istring);
}
