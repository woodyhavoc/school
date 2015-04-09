//TODO: Write the shit described in Job.h
#include "headers.h"
#include "Job.h"

static unsigned int jNum = 0;

JobPtr createJob(pid_t jpid, char* jcmd)
{
	JobPtr job = malloc(sizeof(Job));

	job->pid = jpid;
	job->jobNum = ++jNum;
	job->cmd = malloc(sizeof(strlen(jcmd)));
	
	strcpy(job->cmd, jcmd);

	return job;
}

int compareTo(const void* job1, const void* job2)
{
	JobPtr j1 = (JobPtr)job1;
	JobPtr j2 = (JobPtr)job2;

	return j1->pid == j2->pid;
}

char* toString(const void* job)
{
	JobPtr j = (JobPtr)job;

	char* string = malloc(sizeof(char) * (strlen(j->cmd) + (sizeof(unsigned int)*3)));

	sprintf(string, "[%u] %d %s\n", j->jobNum, (int)j->pid, j->cmd);

	return string;
}

void freeObject(const void* job)
{
	JobPtr j = (JobPtr)job;
	free(j->cmd);
	free(j);
}

void resetJobCount()
{
	jNum = 0;
}

unsigned int getJobCount(JobPtr job)
{
	return job->jobNum;
}
