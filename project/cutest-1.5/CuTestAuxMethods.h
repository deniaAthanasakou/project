#ifndef CU_TEST_AUX_METHODS_H
#define CU_TEST_AUX_METHODS_H
#include "CuTest.h"
#include "../auxMethods.h"

void TestDeleteArrayOfWords(CuTest *tc);
void TestStringToArray(CuTest *tc);
void TestInitialize(CuTest *tc);
void TestBinarySearch(CuTest* tc);
void TestInsertionSort(CuTest* tc);
void TestExecuteQueryFile(CuTest *tc);
void TestCheckIfStringExists(CuTest *tc);
void TestDeletionSort(CuTest *tc);
void TestCallBasicFuncs(CuTest *tc);


CuSuite* AuxMethodsGetSuite();

#endif

