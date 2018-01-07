#ifndef CU_TEST_FUNC_H
#define CU_TEST_FUNC_H
#include "CuTest.h"
#include "../func.h"

#define NOOFBUCKETS 4
#define NOOFCELLS 4

void TestInsert_ngram(CuTest *tc);
void TestSearch_ngram(CuTest *tc);
void TestSearch_ngram_StaticVersion(CuTest *tc);
void TestDelete_ngram(CuTest *tc);


CuSuite* FuncGetSuite();

#endif
