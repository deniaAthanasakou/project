#include <stdio.h>
#include <stdlib.h>
#include "CuTest.h"
#include "CuTestStack.h"
#include "CuTestAuxMethods.h"
#include "CuTestFunc.h"
#include "CuTestStruct.h"
#include "CuTestTopK.h"
#include "CuTestHashTable.h"
#include "CuTestBatchHandler.h"
#include "CuTestBloomFilter.h"
#include "CuTestCompress.h"

CuSuite* CuGetSuite();
CuSuite* CuStringGetSuite();


void RunAllTests(void)
{
	CuString *output = CuStringNew();
	CuSuite* suite = CuSuiteNew();

	//stack
	CuSuite* stackSuite =  StackGetSuite();
	CuSuiteAddSuite(suite,stackSuite);
	free(stackSuite);
	stackSuite=NULL;
	
	//auxMethods
	CuSuite* auxMethodsSuite =  AuxMethodsGetSuite();
	CuSuiteAddSuite(suite, auxMethodsSuite);
	free(auxMethodsSuite);
	auxMethodsSuite=NULL;
	
	//func
	CuSuite* funcSuite =  FuncGetSuite();
	CuSuiteAddSuite(suite, funcSuite);
	free(funcSuite);
	funcSuite=NULL;
	
	//topK
	CuSuite* topKSuite =  TopKGetSuite();
	CuSuiteAddSuite(suite, topKSuite);
	free(topKSuite);
	topKSuite=NULL;
	
	//struct
	CuSuite* structSuite =  StructGetSuite();
	CuSuiteAddSuite(suite, structSuite);
	free(structSuite);
	structSuite=NULL;
	
	CuSuite* batchHandlerSuite =  BatchHandlerGetSuite();
	CuSuiteAddSuite(suite, batchHandlerSuite);
	free(batchHandlerSuite);
	batchHandlerSuite=NULL;
	
	//hashTable
	CuSuite* hashTableSuite =  HashTableGetSuite();
	CuSuiteAddSuite(suite, hashTableSuite);
	free(hashTableSuite);
	hashTableSuite=NULL;


	//compress
	CuSuite* compressSuite =  CompressGetSuite();
	CuSuiteAddSuite(suite, compressSuite);
	free(compressSuite);
	compressSuite=NULL;
	
	//bloomFilter	
	CuSuite* bloomFilterSuite =  BloomFilterGetSuite();
	CuSuiteAddSuite(suite, bloomFilterSuite);
	free(bloomFilterSuite);
	bloomFilterSuite=NULL;

	CuSuiteRun(suite);
	CuSuiteSummary(suite, output);
	CuSuiteDetails(suite, output);
	printf("%s\n", output->buffer);
	

	CuStringDelete(output);
	CuSuiteDelete(suite);
	
}



int main(void)
{
	printf("Beginning of unit testing.\n");
	RunAllTests();
	printf("End of unit testing.\n");
}
