#ifndef CU_TEST_TOP_K_H
#define CU_TEST_TOP_K_H
#include "CuTest.h"
#include "../topK.h"

void TestInitializeTopKArray(CuTest *tc);
void TestInitializeTopKStruct(CuTest *tc);
void TestDoubleTopKArray(CuTest *tc);
void TestInsertTopArray(CuTest *tc);
void TestDestroyTopArray(CuTest *tc);
void TestHeapSort(CuTest *tc);
void TestBinarySearchTopK(CuTest *tc);

CuSuite* TopKGetSuite();

#endif

