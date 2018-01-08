#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "JobScheduler.h"
#include "hashTable.h"


//Job funcs
Job* initializeJob(void* (*functionTemp)(void** parameters),void** parametersTemp){
	Job* job = (Job*)malloc(sizeof(Job));
	job->function = functionTemp;
	job->parameters = parametersTemp;
	job->nextJob = NULL;
	return job;
}

void deleteJob(Job** job){
	
	if((*job)!=NULL){
		if((*job)->parameters!=NULL){
			free((*job)->parameters);
			(*job)->parameters = NULL;
		}
	}
	
	
	if((*job)!=NULL){
		free(*job);
		*job = NULL;
	}	
}

//Queue funcs
Queue* initializeQueue(){
	Queue* q = (Queue*)malloc(sizeof(Queue));
	q->size = 0;
	q->tail = NULL;
	q->head = NULL;
	return q;
}

void pushToQueue(Queue* q,Job* newJob){
	if(newJob!=NULL){
		if(q->head == NULL){			//empty
			q->head = newJob;
			q->tail = newJob;
		}
		else{						//has next
			q->tail->nextJob = newJob;
			q->tail = newJob;
			q->tail->nextJob = NULL;
		}
		q->size++;
	}
}

Job* popFromQueue(Queue* q){
	if(q->head!=NULL){
		Job* head  = q->head;
		q->head = q->head->nextJob;
		q->size--;
		if(q->size == 0)
			q->tail = NULL;
		return head;
	}else{
		q->tail = NULL;
		return NULL;
	}
	
}

void destroyQueue(Queue** q){
	
	while((*q)->size > 0){
		Job* j = popFromQueue(*q);
		deleteJob(&j);
	}
	
	if(*q!=NULL){
		free(*q);
		*q = NULL;
	}
}

void printQueue(Queue* q){
	if(q!=NULL){
		Job* temp = q->head;
		int i=0;
		while(temp!=NULL){
			printf("Job %d\n",i);
			i++;
			temp = temp->nextJob;
		}

		printf("Size is %d\n",q->size);
	}

}

//JobScheduler funcs
JobScheduler* initializeScheduler(int execution_threads){
	JobScheduler* scheduler = (JobScheduler*)malloc(sizeof(JobScheduler));
	scheduler->executionThreads = execution_threads;
	scheduler->jobQueue = initializeQueue();
	scheduler->tids = malloc(execution_threads*sizeof(pthread_t));
	scheduler->submittedAll = 0;
	
	for(int i=0;i<execution_threads;i++){
		if(pthread_create(&scheduler->tids[i],NULL, (void*) executeAllJobs, scheduler)!=0){
			destroyScheduler(&scheduler);
			return NULL;
		}
	}
	
	if(pthread_mutex_init(&(scheduler->lockQueue),NULL)!=0){
		destroyScheduler(&scheduler);
		return NULL;
	}
	if(pthread_cond_init(&(scheduler->notifyThreads),NULL)!=0){
		destroyScheduler(&scheduler);
		return NULL;
	}
	
	return scheduler;
}


void submitJob(JobScheduler* sch, Job* j){
	pthread_mutex_lock(&(sch->lockQueue));
	pushToQueue(sch->jobQueue,j);
	pthread_mutex_unlock(&(sch->lockQueue));
}

void executeAllJobs(JobScheduler* sch){
	
	Job* j;	
	pthread_mutex_lock(&(sch->lockQueue));
	while(sch->jobQueue->size == 0 && sch->submittedAll == 0){ 
		pthread_cond_wait(&(sch->notifyThreads),&(sch->lockQueue));
	}
	pthread_mutex_unlock(&(sch->lockQueue));
	
	for(;;){
		pthread_mutex_lock(&(sch->lockQueue));
		if(sch->jobQueue->head!=NULL){
			j = popFromQueue(sch->jobQueue);
		}
		else{
			pthread_mutex_unlock(&(sch->lockQueue));
			break;
		}
		pthread_mutex_unlock(&(sch->lockQueue));

		void* returnedValue = (j->function(j->parameters));	
		if(returnedValue!=NULL){
			free(returnedValue);
			returnedValue = NULL;
		}
		
		if(j!=NULL){
			deleteJob(&j);
		}
	}
	pthread_join(sch->tids[0],NULL);
}



void waitAllTasksFinish(JobScheduler* sch){ //waits all submitted tasks to finish
	pthread_mutex_lock(&(sch->lockQueue));
	while(sch->jobQueue->size != 0 && sch->submittedAll == 1){
		pthread_mutex_unlock(&(sch->lockQueue));
		pthread_cond_signal(&(sch->notifyThreads));	
		pthread_join(sch->tids[0],NULL);
		sch->submittedAll = 0;
	}
	
	pthread_mutex_unlock(&(sch->lockQueue));
}

void destroyScheduler(JobScheduler** sch){
	
	destroyQueue(&(*sch)->jobQueue);
	
	for(size_t i = 0; i<(*sch)->executionThreads; i++) {
        pthread_join((*sch)->tids[i], NULL);
    }
			
	if((*sch)->tids!=NULL){
		free((*sch)->tids);
		(*sch)->tids = NULL;
	}
	
	(*sch)->executionThreads = -1;
	
	pthread_mutex_lock(&((*sch)->lockQueue));
    pthread_mutex_destroy(&((*sch)->lockQueue));
    pthread_cond_destroy(&((*sch)->notifyThreads));
	
	free((*sch));
	*sch = NULL;
}



