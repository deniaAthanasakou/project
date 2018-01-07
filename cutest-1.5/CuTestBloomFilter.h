#ifndef CU_TEST_BLOOM_FILTER_H
#define CU_TEST_BLOOM_FILTER_H
#include "CuTest.h"
#include "../bloomfilter.h"

void TestInitializeFilter(CuTest *tc);
void TestKthHash(CuTest *tc);
void TestPossiblyContains(CuTest *tc);
void TestFreeFilter(CuTest *tc);

CuSuite* BloomFilterGetSuite();

#endif