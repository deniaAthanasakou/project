#include <stdio.h>
#include <stdlib.h>
#include "CuTestTopK.h"

void TestInitializeTopKArray(CuTest *tc){
	topKArray* topArray = initializeTopKArray();
	CuAssertPtrNotNull(tc,topArray); 	
	CuAssertIntEquals(tc,TOP_SIZE,topArray->length); 
	CuAssertIntEquals(tc,0,topArray->positionInsertion); 
	CuAssertPtrNotNull(tc,topArray->array); 
	
	for(int i=0; i<topArray->length; i++){
		CuAssertPtrEquals(tc,NULL, topArray->array[i].ngram); 
		CuAssertIntEquals(tc,0,topArray->array[i].occurences); 
	}	
	
	destroyTopArray(topArray);
	
}

void TestInitializeTopKStruct(CuTest *tc){
	topKStruct* node=malloc(sizeof(topKStruct));
	initializeTopKStruct(node);
	CuAssertPtrEquals(tc,NULL, node->ngram); 
	CuAssertIntEquals(tc,0,node->occurences); 
	
	free(node);
	node = NULL;
}




CuSuite* TopKGetSuite() {		//adding TestTopK Functions into suite
    CuSuite* suite = CuSuiteNew();
    
    SUITE_ADD_TEST(suite, TestInitializeTopKArray);
    SUITE_ADD_TEST(suite, TestInitializeTopKStruct);
    
    return suite;
}


