#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

void TestDoubleTopKArray(CuTest *tc){
	topKArray *topArray = initializeTopKArray();
	int oldLength = topArray->length;
	doubleTopKArray(topArray);
	int newLength = topArray->length;
	CuAssertIntEquals(tc,2*oldLength,newLength);
	CuAssertPtrNotNull(tc,topArray);
	CuAssertPtrNotNull(tc,topArray->array);
	destroyTopArray(topArray);
	topArray = NULL;
}

void TestInsertTopArray(CuTest *tc){
	topKArray *topArray = initializeTopKArray();
	CuAssertPtrNotNull(tc,topArray);
	char *item = NULL;								//insert NULL item
	insertTopArray(topArray,item);
	CuAssertStrEquals(tc,NULL,topArray->array[topArray->positionInsertion].ngram);
	CuAssertIntEquals(tc,0,topArray->array[topArray->positionInsertion].occurences);
	CuAssertIntEquals(tc,0,topArray->positionInsertion);
	
	
	item = "Hello World";							//insert not null str
	insertTopArray(topArray,item);
	CuAssertStrEquals(tc,"Hello World",topArray->array[topArray->positionInsertion-1].ngram);
	CuAssertIntEquals(tc,1,topArray->array[topArray->positionInsertion-1].occurences);
	CuAssertIntEquals(tc,1,topArray->positionInsertion);
	
	destroyTopArray(topArray);
	topArray = NULL;
}


void TestDestroyTopArray(CuTest *tc){
	topKArray *topArray = NULL;					//null array
	destroyTopArray(topArray);
	CuAssertPtrEquals(tc,NULL,topArray);
	
	topArray = malloc(sizeof(topKArray));		//initialized
	topArray->array = NULL;
	destroyTopArray(topArray);
	topArray = NULL;
	CuAssertPtrEquals(tc,NULL,topArray);
	
	topArray = initializeTopKArray();		//initialized
	destroyTopArray(topArray);
	topArray = NULL;
	CuAssertPtrEquals(tc,NULL,topArray);
	
	topArray = initializeTopKArray();			//with item
	char *item = "Hello World";				
	insertTopArray(topArray,item);
	destroyTopArray(topArray);
	topArray = NULL;
	CuAssertPtrEquals(tc,NULL,topArray);
}

void TestHeapSort(CuTest *tc){
	topKArray *topArray = initializeTopKArray();
	
	topArray->array[0].ngram = malloc((strlen("hello")+1)*sizeof(char));
	strcpy(topArray->array[0].ngram,"hello");
	topArray->array[0].occurences = 2;
	
	topArray->array[1].ngram = malloc((strlen("cat")+1)*sizeof(char));
	strcpy(topArray->array[1].ngram,"cat");
	topArray->array[1].occurences = 1;
	
	topArray->array[2].ngram = malloc((strlen("ant")+1)*sizeof(char));
	strcpy(topArray->array[2].ngram,"ant");
	topArray->array[2].occurences = 10;
	
	topArray->array[3].ngram = malloc((strlen("wax")+1)*sizeof(char));
	strcpy(topArray->array[3].ngram,"wax");
	topArray->array[3].occurences = 1;
	
	topArray->array[4].ngram = malloc((strlen("zero")+1)*sizeof(char));
	strcpy(topArray->array[4].ngram,"zero");
	topArray->array[4].occurences = 4;
	
	topArray->positionInsertion = 5;
	
	//sort for occurences
	HeapSort(topArray->array, topArray->positionInsertion, 0);
	
	CuAssertIntEquals(tc,10,topArray->array[0].occurences);
	CuAssertStrEquals(tc,"ant",topArray->array[0].ngram);
	
	CuAssertIntEquals(tc,4,topArray->array[1].occurences);
	CuAssertStrEquals(tc,"zero",topArray->array[1].ngram);
	
	CuAssertIntEquals(tc,2,topArray->array[2].occurences);
	CuAssertStrEquals(tc,"hello",topArray->array[2].ngram);
	
	CuAssertIntEquals(tc,1,topArray->array[3].occurences);
	CuAssertStrEquals(tc,"cat",topArray->array[3].ngram);
	
	CuAssertIntEquals(tc,1,topArray->array[4].occurences);
	CuAssertStrEquals(tc,"wax",topArray->array[4].ngram);
	
	//sort for ngrams
	HeapSort(topArray->array, topArray->positionInsertion, 1);
	
	CuAssertIntEquals(tc,10,topArray->array[0].occurences);
	CuAssertStrEquals(tc,"ant",topArray->array[0].ngram);
	
	CuAssertIntEquals(tc,1,topArray->array[1].occurences);
	CuAssertStrEquals(tc,"cat",topArray->array[1].ngram);
	
	CuAssertIntEquals(tc,2,topArray->array[2].occurences);
	CuAssertStrEquals(tc,"hello",topArray->array[2].ngram);
	
	CuAssertIntEquals(tc,1,topArray->array[3].occurences);
	CuAssertStrEquals(tc,"wax",topArray->array[3].ngram);
	
	CuAssertIntEquals(tc,4,topArray->array[4].occurences);
	CuAssertStrEquals(tc,"zero",topArray->array[4].ngram);
	
	destroyTopArray(topArray);
	topArray = NULL;
}

void TestBinarySearchTopK(CuTest *tc){
	
	topKArray *topArray = initializeTopKArray();
	
	topArray->array[0].ngram = malloc((strlen("hello")+1)*sizeof(char));
	strcpy(topArray->array[0].ngram,"hello");
	topArray->array[0].occurences = 2;
	
	topArray->array[1].ngram = malloc((strlen("cat")+1)*sizeof(char));
	strcpy(topArray->array[1].ngram,"cat");
	topArray->array[1].occurences = 1;
	
	topArray->array[2].ngram = malloc((strlen("ant")+1)*sizeof(char));
	strcpy(topArray->array[2].ngram,"ant");
	topArray->array[2].occurences = 10;
	
	topArray->array[3].ngram = malloc((strlen("wax")+1)*sizeof(char));
	strcpy(topArray->array[3].ngram,"wax");
	topArray->array[3].occurences = 1;
	
	topArray->array[4].ngram = malloc((strlen("zero")+1)*sizeof(char));
	strcpy(topArray->array[4].ngram,"zero");
	topArray->array[4].occurences = 4;
	
	topArray->positionInsertion = 5;
	
	//sort for ngrams
	HeapSort(topArray->array, topArray->positionInsertion, 1);
	
	//search for 'ant'
	binarySearchTopK(topArray->array, "ant", topArray->positionInsertion);
	CuAssertIntEquals(tc,11,topArray->array[0].occurences);
	
	//search for 'wax'
	binarySearchTopK(topArray->array, "wax", topArray->positionInsertion);
	CuAssertIntEquals(tc,2,topArray->array[3].occurences);
	
	//search for 'zero'
	binarySearchTopK(topArray->array, "zero", topArray->positionInsertion);
	CuAssertIntEquals(tc,5,topArray->array[4].occurences);
	
	//search for something not in array
	binarySearchTopK(topArray->array, "dog", topArray->positionInsertion);
	CuAssertIntEquals(tc,11,topArray->array[0].occurences);
	CuAssertIntEquals(tc,1,topArray->array[1].occurences);
	CuAssertIntEquals(tc,2,topArray->array[2].occurences);
	CuAssertIntEquals(tc,2,topArray->array[3].occurences);
	CuAssertIntEquals(tc,5,topArray->array[4].occurences);
	
	

	
	destroyTopArray(topArray);
	topArray = NULL;

}


CuSuite* TopKGetSuite() {		//adding TestTopK Functions into suite
    CuSuite* suite = CuSuiteNew();
    
    SUITE_ADD_TEST(suite, TestInitializeTopKArray);
    SUITE_ADD_TEST(suite, TestInitializeTopKStruct);
    SUITE_ADD_TEST(suite, TestDoubleTopKArray);
	SUITE_ADD_TEST(suite, TestInsertTopArray);
	SUITE_ADD_TEST(suite, TestDestroyTopArray);
	SUITE_ADD_TEST(suite, TestHeapSort);
	SUITE_ADD_TEST(suite, TestBinarySearchTopK);
	
    return suite;
}


