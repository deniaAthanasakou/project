#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CuTestJobScheduler.h"

//job funcs
void TestInitializeJob(CuTest *tc){
	
	void** p = malloc(sizeof(void*));
	char* str = malloc((strlen("I am a test\n")+1)*sizeof(char));
	strcpy(str,"I am a test\n");
	p[0] = (void*) str;
	Job *j = initializeJob((void*)printf,p);
	CuAssertPtrEquals(tc,(void*)printf,j->function);
	CuAssertPtrEquals(tc,p,j->parameters);
	CuAssertPtrEquals(tc,NULL,j->nextJob);

	deleteJob(&j);
	CuAssertPtrEquals(tc,NULL,j);
	free(str);
	str = NULL;
}

void TestDeleteJob(CuTest *tc){
	void** p = malloc(sizeof(void*));
	char* str = malloc((strlen("I am a test\n")+1)*sizeof(char));
	strcpy(str,"I am a test\n");
	p[0] = (void*) str;
	Job *j = initializeJob((void*)printf,p);
	CuAssertPtrEquals(tc,(void*)printf,j->function);
	CuAssertPtrEquals(tc,p,j->parameters);
	CuAssertPtrEquals(tc,NULL,j->nextJob);

	deleteJob(&j);
	CuAssertPtrEquals(tc,NULL,j);
	free(str);
	str = NULL;
	
	Job *j1  = NULL;
	deleteJob(&j1);
	CuAssertPtrEquals(tc,NULL,j1);
}

//Queue funcs
void TestInitializeQueue(CuTest *tc){
	Queue* q = initializeQueue();
	CuAssertIntEquals(tc,0,q->size);
	CuAssertPtrEquals(tc,NULL,q->tail);
	CuAssertPtrEquals(tc,NULL,q->head);
	
	destroyQueue(&q);
	CuAssertPtrEquals(tc,NULL,q);
}

void TestPushToQueue(CuTest *tc){
	Queue* q = initializeQueue();
	void** p = malloc(sizeof(void*));
	char* str = malloc((strlen("I am a test\n")+1)*sizeof(char));
	strcpy(str,"I am a test\n");
	p[0] = (void*) str;
	Job *j1 = initializeJob((void*)printf,p);
	
	pushToQueue(q,j1);
	CuAssertIntEquals(tc,1,q->size);
	CuAssertPtrEquals(tc,j1,q->tail);
	CuAssertPtrEquals(tc,j1,q->head);
	
	void** p1 = malloc(2*sizeof(void*));
	char* str1 = malloc((strlen("I am a test %d\n")+1)*sizeof(char));
	strcpy(str1,"I am a test\n");
	int no = 1;
	int* noPtr = &no;
	p1[0] = (void*) str;
	p1[1] = (void*) noPtr;
	Job *j2 = initializeJob((void*)printf,p1);
	
	pushToQueue(q,j2);
	CuAssertIntEquals(tc,2,q->size);
	CuAssertPtrEquals(tc,j2,q->tail);
	CuAssertPtrEquals(tc,j1,q->head);
	CuAssertPtrEquals(tc,j2,q->head->nextJob);
	
	Job *j3 = NULL;
	pushToQueue(q,j3);
	CuAssertIntEquals(tc,2,q->size);
	CuAssertPtrEquals(tc,j2,q->tail);
	CuAssertPtrEquals(tc,j1,q->head);
	CuAssertPtrEquals(tc,j2,q->head->nextJob);
	
	free(str);
	str = NULL;
	free(str1);
	str1 = NULL;
	
	destroyQueue(&q);
	
}


void TestPopFromQueue(CuTest *tc){
	Queue* q = initializeQueue();
	void** p = malloc(sizeof(void*));
	char* str = malloc((strlen("I am a test\n")+1)*sizeof(char));
	strcpy(str,"I am a test\n");
	p[0] = (void*) str;
	Job *j1 = initializeJob((void*)printf,p);
	
	pushToQueue(q,j1);
	CuAssertIntEquals(tc,1,q->size);
	CuAssertPtrEquals(tc,j1,q->tail);
	CuAssertPtrEquals(tc,j1,q->head);
	
	void** p1 = malloc(2*sizeof(void*));
	char* str1 = malloc((strlen("I am a test %d\n")+1)*sizeof(char));
	strcpy(str1,"I am a test\n");
	int no = 1;
	int* noPtr = &no;
	p1[0] = (void*) str;
	p1[1] = (void*) noPtr;
	Job *j2 = initializeJob((void*)printf,p1);
	
	pushToQueue(q,j2);
	CuAssertIntEquals(tc,2,q->size);
	CuAssertPtrEquals(tc,j2,q->tail);
	CuAssertPtrEquals(tc,j1,q->head);
	CuAssertPtrEquals(tc,j2,q->head->nextJob);
	
	Job *jPop1 = popFromQueue(q);
	CuAssertIntEquals(tc,1,q->size);
	CuAssertPtrEquals(tc,j2,q->tail);
	CuAssertPtrEquals(tc,j2,q->head);
	CuAssertPtrEquals(tc,NULL,q->head->nextJob);
	
	Job *jPop2 = popFromQueue(q);
	CuAssertIntEquals(tc,0,q->size);
	CuAssertPtrEquals(tc,NULL,q->tail);
	CuAssertPtrEquals(tc,NULL,q->head);
	CuAssertPtrEquals(tc,NULL,q->head);
	
	Job *jPop3 = popFromQueue(q);
	CuAssertIntEquals(tc,0,q->size);
	CuAssertPtrEquals(tc,NULL,q->tail);
	CuAssertPtrEquals(tc,NULL,q->head);
	CuAssertPtrEquals(tc,NULL,q->head);
	
	deleteJob(&jPop1);
	jPop1 = NULL;
	deleteJob(&jPop2);
	jPop2 = NULL;
	deleteJob(&jPop2);
	jPop2 = NULL;
	
	free(str);
	str = NULL;
	free(str1);
	str1 = NULL;
	
	destroyQueue(&q);
}


void TestDestroyQueue(CuTest *tc){
	Queue* q = initializeQueue();
	void** p = malloc(sizeof(void*));
	char* str = malloc((strlen("I am a test\n")+1)*sizeof(char));
	strcpy(str,"I am a test\n");
	p[0] = (void*) str;
	Job *j1 = initializeJob((void*)printf,p);
	
	pushToQueue(q,j1);
	CuAssertIntEquals(tc,1,q->size);
	CuAssertPtrEquals(tc,j1,q->tail);
	CuAssertPtrEquals(tc,j1,q->head);
	
	void** p1 = malloc(2*sizeof(void*));
	char* str1 = malloc((strlen("I am a test %d\n")+1)*sizeof(char));
	strcpy(str1,"I am a test\n");
	int no = 1;
	int* noPtr = &no;
	p1[0] = (void*) str;
	p1[1] = (void*) noPtr;
	Job *j2 = initializeJob((void*)printf,p1);
	
	pushToQueue(q,j2);
	CuAssertIntEquals(tc,2,q->size);
	CuAssertPtrEquals(tc,j2,q->tail);
	CuAssertPtrEquals(tc,j1,q->head);
	CuAssertPtrEquals(tc,j2,q->head->nextJob);
	
	Job *j3 = NULL;
	pushToQueue(q,j3);
	CuAssertIntEquals(tc,2,q->size);
	CuAssertPtrEquals(tc,j2,q->tail);
	CuAssertPtrEquals(tc,j1,q->head);
	CuAssertPtrEquals(tc,j2,q->head->nextJob);
	
	free(str);
	str = NULL;
	free(str1);
	str1 = NULL;
	
	destroyQueue(&q);
	CuAssertPtrEquals(tc,NULL,q);
}

//JobScheduler funcs
void TestInitializeScheduler(CuTest *tc){
	/*JobScheduler* sch = initializeScheduler(1);
	CuAssertIntEquals(tc,1,sch->executionThreads);
	CuAssertIntEquals(tc,0,sch->submittedAll);
	CuAssertPtrNotNull(tc,sch->jobQueue);
	destroyScheduler(&sch);*/
}

void TestSubmitJob(CuTest *tc){
	
}

void TestExecuteAllJobs(CuTest *tc){
	
}
void TestWaitAllTasksFinish(CuTest *tc){

}

void TestDestroyScheduler(CuTest *tc){

}


CuSuite* JobSchedulerGetSuite() {		//adding TestStack Functions into suite
    CuSuite* suite = CuSuiteNew();
    
    SUITE_ADD_TEST(suite, TestInitializeJob);
    SUITE_ADD_TEST(suite, TestDeleteJob);
    SUITE_ADD_TEST(suite, TestInitializeQueue);
    SUITE_ADD_TEST(suite, TestPushToQueue);
    SUITE_ADD_TEST(suite, TestPopFromQueue);
    SUITE_ADD_TEST(suite, TestDestroyQueue);
    SUITE_ADD_TEST(suite, TestInitializeScheduler);
    SUITE_ADD_TEST(suite, TestSubmitJob);
    SUITE_ADD_TEST(suite, TestExecuteAllJobs);
    SUITE_ADD_TEST(suite, TestWaitAllTasksFinish);
    SUITE_ADD_TEST(suite, TestDestroyScheduler);
       
    return suite;
}