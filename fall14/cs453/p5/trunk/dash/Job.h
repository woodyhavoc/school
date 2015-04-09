/**
 * Author: Aaron Woods
 */
#ifndef __JOB_H
#define __JOB_H

#include <common.h>

typedef struct job Job;
typedef struct job* JobPtr;

struct job
{
	pid_t pid;
	unsigned int jobNum;
	char* cmd;
	Boolean reported;
};

/**
 * Creates a Job pointer.  This will later
 * be passed to a Node as its void* obj.
 * It accepts as parameters a pid, and
 * a command given by the user.
 *
 * @return The newly created Job pointer
 */
JobPtr createJob(pid_t jpid, char* jcmd);

/**
 * Compares two Job pointers to each other.
 *
 * @return TRUE if the two pids are equal, FALSE otherwise
 */
int compareTo(const void* job1, const void* job2);

/**
 * Returns a meaningful representation of the Job pointer.
 *
 * @param job Job pointer which will have representative string
 * 			  made.
 * @return string representation of the Job pointer.
 */
char* toString(const void* job);

/**
 * Frees the memory of the Job pointer.
 *
 * @param job Job pointer to be freed.
 */
void freeObject(const void* job);

/**
 * Resets the global job count.
 */
void resetJobCount();

/**
 * Returns the current job count.
 *
 * @param job Job pointer used to access the global
 * 			  job count variable.
 */
unsigned int getJobCount(JobPtr job);

Boolean wasReported(JobPtr job);

void setReported(JobPtr job);

#endif /* __JOB_H */
