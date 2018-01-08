#ifndef _JOB_SCHEDULER_H_
#define _JOB_SCHEDULER_H_

#include <pthread.h>
#include <stdbool.h>

typedef struct params{
	void* parameter;
}params;

typedef struct Job{
	struct Job* nextJob;
	void* (*function)(void** parameters);
	void** parameters;
}Job;


typedef struct Queue{
	Job* head;
	Job* tail;
	int size;
}Queue;

typedef struct JobScheduler{
	int executionThreads;
	Queue* jobQueue; 
	pthread_t* tids; // execution threads
	pthread_cond_t notifyThreads;
	pthread_mutex_t lockQueue;
	int submittedAll;
}JobScheduler;

//Job funcs
Job* initializeJob(void* (*function)(void** parameters),void** parameters);
void deleteJob(Job** job);

//Queue funcs
Queue* initializeQueue();
void pushToQueue(Queue* q,Job* newJob);
Job* popFromQueue(Queue* q);
void destroyQueue(Queue** q);
void printQueue(Queue* q);

//JobScheduler funcs
JobScheduler* initializeScheduler(int execution_threads);
void submitJob(JobScheduler* sch, Job* j);
void executeAllJobs(JobScheduler* sch);
void waitAllTasksFinish(JobScheduler* sch); //waits all submitted tasks to finish
void destroyScheduler(JobScheduler** sch);

#endif