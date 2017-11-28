#ifndef CU_TEST_STACK_H
#define CU_TEST_STACK_H
#include "CuTest.h"
#include "../stack.h"

void TestInitializeStack(CuTest *tc);
void TestDeleteStack(CuTest *tc);
void TestDoubleLengthOfStack(CuTest *tc);
void TestPush(CuTest *tc);
void TestIsEmpty(CuTest *tc);
void TestPop(CuTest *tc);


CuSuite* StackGetSuite();

#endif

