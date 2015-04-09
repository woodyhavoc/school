
#ifndef __EXAMPLE_H
#define __EXAMPLE_H
/*
 * example.h -- definitions for the char module
 *
 */
#ifndef BOOGA_MAJOR
#define BOOGA_MAJOR 0   /* dynamic major by default */
#endif

#ifndef BOOGA_NR_DEVS
#define BOOGA_NR_DEVS 4    /* example0 through example3 */
#endif
/*
 * Split minors in two parts
 */
#define TYPE(dev)   (MINOR(dev) >> 4)  /* high nibble */
#define NUM(dev)    (MINOR(dev) & 0xf) /* low  nibble */

/*
 * The different configurable parameters
 */
struct booga_stats {
	long int num_open;
	long int num_read; 
	long int num_write; 
	long int num_close;
	long int bytes_read;
	long int bytes_written; 
	struct semaphore sem;
};
typedef struct booga_stats booga_stats;

struct booga_dev
{
	char *data;
	unsigned int number;
};

typedef struct booga_dev booga_dev;

/*extern example_stats Example_Device_Stats;*/

#endif /* __EXAMPLE_H */
