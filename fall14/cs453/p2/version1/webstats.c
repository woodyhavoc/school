
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

/*
	Generate some web statistics  based on access logs from the Apache web server. 
	In particular, this program looks for local versus total get requests. 
	It also analyzes failed gets and total bytes transferred.

	The format of a Apache web server access log is as follows:

	For a successful get:

	1cust107.tnt5.phoenix.az.da.uu.net - - [13/Mar/2001:15:45:45 -0700] 
                                             "GET /gas/gif/lib.gif HTTP/1.1" 200 1138

	The last field tells the number of bytes that were transferred. The second
	last field tells the status of the request: in this case 200 says it was 
	successful. A value of 404 implies that the object requested was not
	found.

	For an unsuccessful get:

	j3009.inktomi.com - - [11/Mar/2001:04:07:31 -0700] "GET /~kani HTTP/1.0" 404 278

	Using the webstats program: The following show various ways of invoking the
	webstats  program.

		webstats access_log

		webstats access_log.1 access_log.2

		webstats access_log.* 

	author: Amit Jain
*/

#define MAX_LINE_SIZE 4096
#define MAX_NUM_FIELDS 40

#define TRUE 1
#define FALSE 0

typedef int Boolean;

struct stats {
	double local_bytes;
	double total_bytes;
	double local_gets;
	double total_gets;
	double failed_gets;
	double local_failed_gets;
	char **url;
	int *requests;
	char* filename;
} webstats;

char *program_name;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

Boolean locked = FALSE;

int numThread, numCPU;

void* process_file(void* ptr);
void mutexLock();
void mutexUnlock();
Boolean mutexIsLocked();

/*
 * parse_line(): Parse one line into string tokens separated by the given delimiters.
 * Beware: The input argument line is consumed in the process.
 *
 *	input:
 * 		line: A character array containing the current server log entry line
 * 		delim: A character array containing the delimiters to be used to separate fields in the line
 *
 * 	output:
 * 		field: An array of strings that represent the tokens found in the line.
 *
 * 	return:
 * 		num: The number of tokens found in the line.
 */

static int parse_line(char *line, const char *delim, char *field[])
{
	char *next;
	int cnt=0;
	char* savePtr = NULL;	
	next = strtok_r(line, delim, &savePtr);
	while (next) {
		if (cnt == MAX_NUM_FIELDS-1) break;
		field[cnt] = (char *) malloc(strlen(next)+1);
		strcpy(field[cnt++],next);
		next = strtok_r(NULL, delim, &savePtr);
	}
	field[cnt] = (char *) 0; /* make the field array be null-terminated */

	return cnt;
}


/*
 * free_tokens(): Frees spaces allocated for tokens in parse_line
 *
 *	input:
 *	    field: an array of strings
 * 		num: An int representing number of tokens in array of strings named * 		field.
 *
 * 	output:
 *
 * 	return:
 * 		none.
 */
static void free_tokens(int num, char *field[])
{
	int i;
	for (i=0; i<num; i++)
	{
		free(field[i]);
	}
}



/*
 * initialize_webstats(): Initialize the webstats structure.
 *
 * 	input:	
 * 		none
 *	output
 * 		none
 * 	return
 * 		none
 */

static void initialize_webstats()
{
	webstats.total_bytes = 0;
	webstats.total_gets = 0;
	webstats.local_bytes = 0;
	webstats.local_gets = 0;
	webstats.failed_gets = 0;
	webstats.local_failed_gets = 0;
	webstats.filename = NULL;
}

static void initWebstatsPtr(struct stats* webstat, char* arg)
{
	webstat->total_bytes = 0;
	webstat->total_gets = 0;
	webstat->local_bytes = 0;
	webstat->local_gets = 0;
	webstat->failed_gets = 0;
	webstat->local_failed_gets = 0;
	webstat->filename = arg;
}


/*
 *	update_webstats(): Updates the webstats structure based on the input fields of current line.
 *
 *	input: 
 *		num: The number of fields in the current webserver log entry
 *		field: An array of num strings representing the log entry
 *
 *	output: 
 *		none
 */
#define BYTES_DOWNLOADED_FIELD 9
#define HTTP_STATUS_CODE_FIELD 8
static void update_webstats(int num, char **field, struct stats* webstat)
{
	int bytes_downloaded = atoi(field[BYTES_DOWNLOADED_FIELD]);

	webstat->total_gets++;
	webstat->total_bytes += bytes_downloaded;
	if (atoi(field[HTTP_STATUS_CODE_FIELD]) == 404) webstat->failed_gets++;

	if ((strstr(field[0], "boisestate.edu") != NULL) || (strstr(field[0], "132.178") != NULL))
	{
		webstat->local_gets++;
		webstat->local_bytes += bytes_downloaded;
		if (atoi(field[HTTP_STATUS_CODE_FIELD]) == 404)
			webstat->local_failed_gets++;
	}
}

static void updateGlobalWebstats(struct stats* webstat)
{
	pthread_mutex_lock(&mutex);
	webstats.total_bytes += webstat->total_bytes;
	webstats.total_gets += webstat->total_gets;
	webstats.local_bytes += webstat->local_bytes;
	webstats.local_gets += webstat->local_gets;
	webstats.failed_gets += webstat->failed_gets;
	webstats.local_failed_gets += webstat->local_failed_gets;
	pthread_mutex_unlock(&mutex);
}


/*
 * 	print_webstats(): Print out webstats on standard output.
 *
 *	input: 
 *		none
 *	output: 
 *		none
 *
 *	return
 *		none
 */
static void print_webstats()
{
	printf("%10s %15s   %15s  %15s\n", "TYPE", "gets","failed gets",
													"MB transferred");
	printf("%10s  %15.0f  %15.0f  %15.0f\n", "local", webstats.local_gets, 
		webstats.local_failed_gets, (double) webstats.local_bytes/(1024*1024));
	printf("%10s  %15.0f  %15.0f  %15.0f\n", "total", webstats.total_gets, 
		webstats.failed_gets, (double) webstats.total_bytes/(1024*1024));
}


/*
 * process_file(): The main function that processes one log file
 *
 *	input:
 * 		ptr: Pointer to log file name.
 *
 *	output:
 *		none
 *	return
 *		none
 */
void* process_file(void *ptr)
{
	struct stats* webstat = (struct stats*)ptr;
	char *linebuffer = (char *) malloc(sizeof(char) * MAX_LINE_SIZE);
	char **field = (char **) malloc(sizeof(char *) * MAX_NUM_FIELDS);
	char *end_date = (char *) malloc(sizeof(char) * MAX_LINE_SIZE);

	fprintf(stderr,"%s: processing log file %s\n", program_name, webstat->filename);
	FILE *fin = fopen(webstat->filename,"r");
	if (fin == NULL)
	{
		fprintf(stderr,"Cannot open file %s\n", webstat->filename);
		exit(1);
	}
	char *s = fgets(linebuffer, MAX_LINE_SIZE, fin);
	if (s != NULL)
	{
		int num = parse_line(linebuffer, " []\"", field);
		update_webstats(num, field, webstat);
		printf("Starting date: %s\n",field[3]);
		free_tokens(num, field);

		while (fgets(linebuffer, MAX_LINE_SIZE, fin) != NULL)
		{
			int num = parse_line(linebuffer, " []\"", field);
			strcpy(end_date, field[3]);
			update_webstats(num, field, webstat);
			free_tokens(num, field);
			strcpy(linebuffer,"");
		}
		printf("Ending date: %s\n", end_date);
		free(end_date);
		free(linebuffer);
		free(field);
	}
	else
	{
		free(end_date);
		free(linebuffer);
		free(field);
	}

	updateGlobalWebstats(webstat);
	exit(0);	
	return (void*)0;
}

int main(int argc, char **argv)
{
	int i;

	if (argc <  2) {
		fprintf(stderr,"Usage: %s <access_log_file> {<access_log_file>} \n",argv[0]);
		exit(1);
	}

	program_name = (char *) malloc(strlen(argv[0])+1);
	strcpy(program_name, argv[0]);

	initialize_webstats();
	
	pthread_t* threads = (pthread_t*)malloc(sizeof(pthread_t) * (argc-1));

	struct stats** statsPtr = (struct stats**)malloc(sizeof(struct stats*) * (argc - 1));
		
	pthread_mutex_init(&mutex, NULL);
	
	for(i = 1; i < argc; i++)
	{
		statsPtr[i-1] = (struct stats*)malloc(sizeof(struct stats));
		initWebstatsPtr(statsPtr[i-1], argv[i]);
		pthread_create(&threads[i-1], NULL, process_file, statsPtr[i-1]);
	}
	
	for(i = 1; i < argc; i++)
	{
		pthread_join(threads[i-1], NULL);
	}

	print_webstats();
	pthread_mutex_destroy(&mutex);

	for(i = 0; i < argc; i++)
	{
		free(statsPtr[i]);
	}
		
	free(statsPtr);
	free(threads);
	exit(0);
}

/* vim: set ts=4: */
