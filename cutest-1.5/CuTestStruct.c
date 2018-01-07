#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CuTestStruct.h"

void TestInitializeArray(CuTest *tc){
	arrayOfStructs* array_of_str = malloc(sizeof(arrayOfStructs));
	initializeArray(array_of_str);
	
	CuAssertIntEquals(tc,10,array_of_str->length);
	CuAssertIntEquals(tc,0,array_of_str->position);
	CuAssertPtrNotNull(tc,array_of_str->array);
	
	for(int i=0;i<array_of_str->length;i++){
	
		CuAssertPtrEquals(tc,NULL,array_of_str->array[i].dynamicWord);
		CuAssertPtrEquals(tc,NULL,array_of_str->array[i].nextWordArray);
		CuAssertTrue(tc,!array_of_str->array[i].isDynamic);
		CuAssertTrue(tc,!array_of_str->array[i].isFinal);
		CuAssertTrue(tc,array_of_str->array[i].isEmpty);
		CuAssertIntEquals(tc,-1,array_of_str->array[i].noOfChars);
		CuAssertIntEquals(tc,20,sizeof(array_of_str->array[i].word));
		CuAssertIntEquals(tc,0,strlen(array_of_str->array[i].word));
		CuAssertStrEquals(tc,"\0",array_of_str->array[i].word);
		CuAssertPtrEquals(tc,NULL,array_of_str->array[i].staticArray);
		CuAssertIntEquals(tc,0,array_of_str->array[i].staticArrayLength);
		CuAssertIntEquals(tc,0,array_of_str->array[i].additionVersion);
		CuAssertIntEquals(tc,-1,array_of_str->array[i].deletionVersion);
		CuAssertIntEquals(tc,-1,array_of_str->array[i].notFinalInDeletionVersion);
		CuAssertIntEquals(tc,-1,array_of_str->array[i].finalSinceAdditionVersion);
		
	}
	
	deleteArray(array_of_str);
	array_of_str = NULL;
}

void TestInitializeDataNode(CuTest *tc){
	dataNode* node = malloc(sizeof(dataNode));
	initializeDataNode(node);
	
	
	CuAssertPtrEquals(tc,NULL,node->dynamicWord);
	CuAssertPtrEquals(tc,NULL,node->nextWordArray);
	CuAssertTrue(tc,!node->isDynamic);
	CuAssertTrue(tc,!node->isFinal);
	CuAssertTrue(tc,node->isEmpty);
	CuAssertIntEquals(tc,-1,node->noOfChars);
	CuAssertIntEquals(tc,20,sizeof(node->word));
	CuAssertIntEquals(tc,0,strlen(node->word));
	CuAssertStrEquals(tc,"\0",node->word);
	CuAssertPtrEquals(tc,NULL,node->staticArray);
	CuAssertIntEquals(tc,0,node->additionVersion);
	CuAssertIntEquals(tc,-1,node->deletionVersion);
	CuAssertIntEquals(tc,0,node->staticArrayLength);
	CuAssertIntEquals(tc,-1,node->notFinalInDeletionVersion);
	CuAssertIntEquals(tc,-1,node->finalSinceAdditionVersion);

	
	
	deleteDataNode(node);
	free(node);
	node = NULL;
}


void TestDoubleLength(CuTest *tc){
	arrayOfStructs* array_of_str = malloc(sizeof(arrayOfStructs));
	initializeArray(array_of_str);
	int lengthBefore = array_of_str->length;
	doubleLength(array_of_str);
	CuAssertIntEquals(tc,lengthBefore * 2,array_of_str->length);
	CuAssertPtrNotNull(tc,array_of_str->array);
	deleteArray(array_of_str);
	array_of_str = NULL;
}


void TestDeleteArray(CuTest *tc){
	arrayOfStructs* array_of_str = NULL;					//when array_of_str is null
	deleteArray(array_of_str);
	CuAssertPtrEquals(tc,NULL,array_of_str);
	array_of_str = malloc(sizeof(arrayOfStructs));			//when array_of_str is not initialized 
	array_of_str->array = NULL;
	array_of_str->position = 0;
	array_of_str->length = 0;
	deleteArray(array_of_str);
	array_of_str = NULL;
	CuAssertPtrEquals(tc,NULL,array_of_str);
	
	array_of_str = malloc(sizeof(arrayOfStructs));			//when initialized
	initializeArray(array_of_str);
	deleteArray(array_of_str);
	array_of_str = NULL;
	CuAssertPtrEquals(tc,NULL,array_of_str);
	
}


void TestDeleteDataNode(CuTest *tc){
	
	int k = 15;											//word null and staticArray null
	dataNode* elem = malloc(sizeof(dataNode));
	elem->dynamicWord = NULL;
	elem->staticArray = NULL;
	deleteDataNode(elem);
	CuAssertPtrEquals(tc,NULL,elem->dynamicWord);
	CuAssertPtrEquals(tc,NULL,elem->staticArray);
	free(elem);
	elem = NULL;

	elem = malloc(sizeof(dataNode));					//word not null and staticArray null
	elem->dynamicWord = malloc(k * sizeof(char));
	strcpy(elem->dynamicWord,"Hello world");
	elem->staticArray = NULL;
	deleteDataNode(elem);
	CuAssertPtrEquals(tc,NULL,elem->dynamicWord);
	CuAssertPtrEquals(tc,NULL,elem->staticArray);
	free(elem);
	elem = NULL;
	
	elem = malloc(sizeof(dataNode));					//word null but staticArray not null
	elem->dynamicWord = NULL;
	elem->staticArray = malloc(3 * sizeof(int));
	elem->staticArray[0] = 3;
	elem->staticArray[1] = -5;
	elem->staticArray[2] = 10;
	deleteDataNode(elem);
	CuAssertPtrEquals(tc,NULL,elem->dynamicWord);
	CuAssertPtrEquals(tc,NULL,elem->staticArray);
	free(elem);
	elem = NULL;
	
	elem = malloc(sizeof(dataNode));					//word not null and staticArray not null
	elem->dynamicWord = malloc(k * sizeof(char));
	strcpy(elem->dynamicWord,"Hello world");
	elem->staticArray = malloc(3 * sizeof(int));
	elem->staticArray[0] = 3;
	elem->staticArray[1] = -5;
	elem->staticArray[2] = 10;
	deleteDataNode(elem);
	CuAssertPtrEquals(tc,NULL,elem->dynamicWord);
	CuAssertPtrEquals(tc,NULL,elem->staticArray);
	free(elem);
	elem = NULL;
}

void TestDeleteArray1Layer(CuTest *tc){

	arrayOfStructs* array_of_str = NULL;					//when array_of_str is null
	deleteArray(array_of_str);
	CuAssertPtrEquals(tc,NULL,array_of_str);
	array_of_str = malloc(sizeof(arrayOfStructs));			//when array_of_str is not initialized 
	array_of_str->array = NULL;
	array_of_str->position = 0;
	array_of_str->length = 0;
	deleteArray(array_of_str);
	array_of_str = NULL;
	CuAssertPtrEquals(tc,NULL,array_of_str);
	
	array_of_str = malloc(sizeof(arrayOfStructs));			//when initialized
	initializeArray(array_of_str);
	deleteArray(array_of_str);
	array_of_str = NULL;
	CuAssertPtrEquals(tc,NULL,array_of_str);

}




CuSuite* StructGetSuite() {		//adding TestStruct Functions into suite
    CuSuite* suite = CuSuiteNew();
  
    SUITE_ADD_TEST(suite, TestInitializeArray);
    SUITE_ADD_TEST(suite, TestInitializeDataNode);
	SUITE_ADD_TEST(suite, TestDoubleLength);
	SUITE_ADD_TEST(suite, TestDeleteArray);
	SUITE_ADD_TEST(suite, TestDeleteDataNode);
	SUITE_ADD_TEST(suite, TestDeleteArray1Layer);

    
    return suite;
}
