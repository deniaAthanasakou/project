#ifndef CU_TEST_COMPRESS_H
#define CU_TEST_COMPRESS_H
#include "CuTest.h"
#include "../compress.h"

void TestRecreateStructure_RecursiveCompression(CuTest *tc);
void TestCheckForCompression(CuTest *tc);
void TestCompress(CuTest *tc);
void TestGetNgramFromNode(CuTest *tc);


CuSuite* CompressGetSuite();

#endif