#ifndef CU_TEST_STRUCT_H
#define CU_TEST_STRUCT_H
#include "CuTest.h"
#include "../struct.h"

void TestInitializeArray(CuTest *tc);
void TestDoubleLength(CuTest *tc);
void TestDeleteArray(CuTest *tc);
void TestDeleteDataNode(CuTest *tc);

CuSuite* StructGetSuite();

#endif