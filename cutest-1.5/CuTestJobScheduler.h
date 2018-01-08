#ifndef CU_TEST_JOB_SCHEDULER_H
#define CU_TEST_JOB_SCHEDULER_H
#include "CuTest.h"
#include "../JobScheduler.h"

//job funcs
void TestInitializeJob(CuTest *tc);
void TestDeleteJob(CuTest *tc);

//Queue funcs
void TestInitializeQueue(CuTest *tc);
void TestPushToQueue(CuTest *tc);
void TestPopFromQueue(CuTest *tc);
void TestDestroyQueue(CuTest *tc);

//JobScheduler funcs
void TestInitializeScheduler(CuTest *tc);
void TestSubmitJob(CuTest *tc);
void TestExecuteAllJobs(CuTest *tc);
void TestWaitAllTasksFinish(CuTest *tc);
void TestDestroyScheduler(CuTest *tc);

CuSuite* JobSchedulerGetSuite();

#endif