#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CuTestStruct.h"

void TestInitializeArray(CuTest *tc){
	arrayOfStructs* array_of_str = malloc(sizeof(arrayOfStructs));
	array_of_str->position=0;
	array_of_str->length=10;
	array_of_str->array=malloc(array_of_str->length * sizeof(dataNode));
	
	for(int i=0;i<array_of_str->length;i++){
		array_of_str->array[i].word = NULL;
		array_of_str->array[i].nextWordArray = NULL;
	}
	
	CuAssertIntEquals(tc,10,array_of_str->length);
	CuAssertIntEquals(tc,0,array_of_str->position);
	CuAssertPtrNotNull(tc,array_of_str->array);
	for(int i=0;i<array_of_str->length;i++){
		CuAssertPtrEquals(tc,NULL,array_of_str->array[i].word);
		CuAssertPtrEquals(tc,NULL,array_of_str->array[i].nextWordArray);
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
	elem->word = NULL;
	deleteDataNode(elem);
	CuAssertPtrEquals(tc,NULL,elem->word);
	free(elem);
	elem = NULL;

	elem = malloc(sizeof(dataNode));					//word not null
	elem->word = malloc(k * sizeof(char));
	strcpy(elem->word,"Hello world");
	deleteDataNode(elem);
	CuAssertPtrEquals(tc,NULL,elem->word);
	free(elem);
	elem = NULL;
	
}




CuSuite* StructGetSuite() {		//adding TestStack Functions into suite
    CuSuite* suite = CuSuiteNew();
  
    SUITE_ADD_TEST(suite, TestInitializeArray);
	SUITE_ADD_TEST(suite, TestDoubleLength);
	SUITE_ADD_TEST(suite, TestDeleteArray);
	SUITE_ADD_TEST(suite, TestDeleteDataNode);
    
    return suite;
}