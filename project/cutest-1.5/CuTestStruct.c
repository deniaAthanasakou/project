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
		CuAssertIntEquals(tc,-1,array_of_str->array[i].noOfChars);
		CuAssertIntEquals(tc,20,sizeof(array_of_str->array[i].word));
		CuAssertIntEquals(tc,0,strlen(array_of_str->array[i].word));
		
	}
	
	deleteArray(array_of_str);
	array_of_str = NULL;
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
	
	int k = 15;											//word null
	dataNode* elem = malloc(sizeof(dataNode));
	elem->dynamicWord = NULL;
	deleteDataNode(elem);
	CuAssertPtrEquals(tc,NULL,elem->dynamicWord);
	free(elem);
	elem = NULL;

	elem = malloc(sizeof(dataNode));					//word not null
	elem->dynamicWord = malloc(k * sizeof(char));
	strcpy(elem->dynamicWord,"Hello world");
	deleteDataNode(elem);
	CuAssertPtrEquals(tc,NULL,elem->dynamicWord);
	free(elem);
	elem = NULL;
	
}




CuSuite* StructGetSuite() {		//adding TestStruct Functions into suite
    CuSuite* suite = CuSuiteNew();
  
    SUITE_ADD_TEST(suite, TestInitializeArray);
	SUITE_ADD_TEST(suite, TestDoubleLength);
	SUITE_ADD_TEST(suite, TestDeleteArray);
	SUITE_ADD_TEST(suite, TestDeleteDataNode);
    
    return suite;
}
