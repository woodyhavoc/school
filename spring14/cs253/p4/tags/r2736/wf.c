#include <string.h>
#include <stdio.h>

#include "doublyLinkedLists/List.h"
#include "doublyLinkedLists/Node.h"
#include "doublyLinkedLists/common.h"
#include "HashTable.h"
#include "WordObj.h"

void checkArgs(int argc, char *argv[]);
void printUsage();
void fillTableFile(HashTablePtr table, char * argv[]);
void fillTableSTD(HashTablePtr table);

const char* delimiters = " \t;{}()[].#<>\n\r+-/%*\"^'~&=!|:\\?,";

int main(int argc, char *argv[])
{
    checkArgs(argc, argv);

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

void checkArgs(int argc, char *argv[])
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

	size_t size = 1000;

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
	}
	
	free(istring);

	fclose(file);
}

void fillTableSTD(HashTablePtr table)
{
	size_t size = 1000;

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
	}

	free(istring);
}
