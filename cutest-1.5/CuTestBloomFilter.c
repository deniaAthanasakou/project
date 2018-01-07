#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../bloomfilter.h"
#include "CuTestBloomFilter.h"
#define NUM_HASHES 5


void TestInitializeFilter(CuTest *tc){
	BloomFilter *filter = NULL;
	filter = initializeFilter(NUM_HASHES);
	CuAssertPtrNotNull(tc,filter);
	for(int i=0;i<HASH_SIZE;i++){
		CuAssertTrue(tc,!filter->bitVector[i]);
	}
	
	freeFilter(filter);
}


void TestKthHash(CuTest *tc){
	uint8_t k = 10;
	uint64_t hashA = 20;
	uint64_t hashB = 15;
	uint64_t filterSize = 200;
	uint64_t result = kthHash(k,hashA,hashB,filterSize);
	CuAssertIntEquals(tc,170,result);
	
	filterSize = 50;
	result = kthHash(k,hashA,hashB,filterSize);
	CuAssertIntEquals(tc,20,result);
}


void TestPossiblyContains(CuTest *tc){
	BloomFilter *filter = NULL;
	filter = initializeFilter(NUM_HASHES);
	bool result = possiblyContains(filter,"hello", strlen("hello"));
	CuAssertTrue(tc,!result);
	
	addFilter(filter,"hello", strlen("hello"));
	
	result = possiblyContains(filter,"hello", strlen("hello"));
	CuAssertTrue(tc,result);
	
	freeFilter(filter);
}

void TestFreeFilter(CuTest *tc){
	BloomFilter *filter = NULL;
	filter = initializeFilter(NUM_HASHES);
	CuAssertPtrNotNull(tc,filter);
	
	freeFilter(filter);
	filter = NULL;
	CuAssertPtrEquals(tc,NULL,filter);
	
}


CuSuite* BloomFilterGetSuite() {		//adding TestBloomFilter Functions into suite
    CuSuite* suite = CuSuiteNew();
    
    SUITE_ADD_TEST(suite, TestInitializeFilter);
	SUITE_ADD_TEST(suite, TestKthHash);
    SUITE_ADD_TEST(suite, TestPossiblyContains);
	SUITE_ADD_TEST(suite, TestFreeFilter);
	
    return suite;
}

