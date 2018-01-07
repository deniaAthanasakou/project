#ifndef CU_TEST_AUX_METHODS_H
#define CU_TEST_AUX_METHODS_H
#include "CuTest.h"
#include "../auxMethods.h"

#define NOOFBUCKETS 4
#define NOOFCELLS 4

void TestStringToArray(CuTest *tc);
void TestInitialize(CuTest *tc);
void TestBinarySearch(CuTest* tc);
void TestInsertionSort(CuTest* tc);
void TestExecuteQueryFile_ExecuteDynamicArray(CuTest *tc);
void TestDeletionSort(CuTest *tc);
void TestGetString(CuTest *tc);
void TestInsertString (CuTest *tc);
void TestBinarySearchBucket(CuTest* tc);
void TestInsertionSortBucket(CuTest* tc);
void TestCopyDataNode(CuTest* tc);
void TestHasChildren(CuTest* tc);

CuSuite* AuxMethodsGetSuite();

#endif

