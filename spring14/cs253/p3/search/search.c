/**
 * search -- Search a set of numbers. 			
 *							
 * Usage:						
 *	search						
 *		You will be asked numbers to lookup	
 *							
 * Files:						
 *	numbers.txt -- numbers 1 per line to search	
 *			(Numbers must be sorted)	
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_NUMBERS	 1000	/* Max numbers in file */

enum boolean {FALSE, TRUE};
typedef enum boolean Boolean;

const char DATA_FILE[] = "numbers.txt";	/* File with numbers */

int data[MAX_NUMBERS];	/* Array of numbers to search */
int max_count;		/* Number of valid elements in data */

Boolean isDigit(char val);
void checkLetters(char string[]);

/**
* Read sorted number from the input file.
*
* @return 
*/
Boolean read_data() 
{
    FILE *in_file;	/* Input file */
    char line[80];	/* Input line */

    in_file = fopen(DATA_FILE, "r");
    if (in_file == NULL) {
		return FALSE;
    }

    /*
     * Read in data 
     */

    max_count = 0;
    while (1) {
		if (fgets(line, sizeof(line),  in_file) == NULL)
	    	break;

		/* convert number */
		sscanf(line, "%d", &data[max_count]); // Fix #1 - See README
		++max_count;
    }

	fclose(in_file);
	return TRUE;
}


/**
* Do a binary search on the data array to search for the given key value.
*
* @param low   starting index for binary search
* @param high  ending index for binary search
* @param data  the sorted srray to search in
* @param key   the key value to search for
*
* @return       the index where the key is found, -1 if not found
*/
int binary_search(int low, int high, int *data, int key) 
{
    int	middle;		/* Middle of our search range */

	while (1) {
    	middle = (low + high) / 2;

    	if (data[middle] == key) {
			return middle;
    	}

    	if (low == high) {
			return -1;
    	}

    	if (data[middle] < key)
			low = middle;
    	else
			high = middle;

		if(high - low == 1)
		{
			if(data[high] == key)
			{
				return high;
			}
			else if(data[low] == key)
			{
				return low;
			}
			else
			{
				return -1;
			}
		}
	
	}
	return -1;
}



int main(int argc, char **argv)
{
    int key;		/* number to search for */
	int index;
    char line[80];	/* Input line */

	if (!read_data()) {
		fprintf(stderr,"Error: Unable to open %s\n", DATA_FILE);
		exit(1);
	}

    while (1) {
		printf("Enter number to search for or -1 to quit: ");
		fgets(line, sizeof(line), stdin);
		sscanf(line, "%d", &key);
		
		if(!isDigit(line[0]) && key != -1)
		{
			printf("Invalid input given.\n");
			continue;
		}	
		
		if(key == -1)
		{
    		break;
		}

		checkLetters(line);

		index = binary_search(0, max_count, data, key);
		
		if(index == -1)
		{
			printf("search: key %d not found\n", key);
		}
		else if(index >= 0)
		{
			printf("search: key %d found at data[%d]\n", key, index);
		}

   }
   return (0);
}

Boolean isDigit(char val)
{
	return (val < '0' || val > '9') ? FALSE : TRUE;
}

void checkLetters(char string[])
{
	int i;
	int size = strlen(string);

	for(i = 0; i < size-1; i++)
	{
		if(!isDigit(string[i]) && string[i] != '\0')
		{
			printf("Ignoring all after \"%c\"\n", string[i-1]);
			break;
		}
	}
}
