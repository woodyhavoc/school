#ifndef __JOB_H
#define __JOB_H

#include <stdio.h>
#include <stdlib.h>
#include "common.h"

#define MAXPID_DIGITS 20


typedef struct job Job;
typedef struct job *JobPtr;

struct job {
    int jobid;
    char *info;
};

/**
 * Creates a Job Pointer with an integer key value,
 * and a string value for the description.
 *
 * @param jobid, integer key value
 * @param info, string value for the description
 *
 * @return newly created job pointer 
 */
JobPtr createJob(int jobid, char * info);

/**
 * Frees the memory allocated to the job pointer
 * given as the parameter.
 *
 * @param job, job pointer whose allocated memory will be freed
 */
void freeJob(JobPtr job);

/**
 * Creates a string using the data members
 * of the job pointer passed as a parameter.
 *
 * @param job, job pointer to be represented by a string
 *
 * @return string representation of the job pointer
 */
char *toString(JobPtr job);


#endif /* __JOB_H */
