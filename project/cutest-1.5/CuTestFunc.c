#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CuTestFunc.h"

void TestDelete_ngram(CuTest *tc){

	/*
		D a
		A test it
		A this is cat
		A this is fox
		D test it
		A test
		D this is
	*/
	
	arrayOfStructs* array_of_str = malloc(sizeof(arrayOfStructs));
	initializeArray(array_of_str);
	
	char* myString= malloc(sizeof(char)*(strlen("a")+1));
	strcpy(myString,"a");
	callBasicFuncs(myString,array_of_str,'D');			//will not be deleted because array_of_str does not have any ngrams inside yet
	free(myString);
	
	
	CuAssertPtrNotNull(tc,array_of_str);
	CuAssertIntEquals(tc,0,array_of_str->position);	
	CuAssertIntEquals(tc,10,array_of_str->length);	
	CuAssertPtrNotNull(tc,array_of_str->array);
	CuAssertPtrEquals(tc,NULL,array_of_str->array[array_of_str->position].word);
	
	CuAssertPtrEquals(tc,NULL,array_of_str->array[array_of_str->position].nextWordArray);
	
	
	myString = malloc(sizeof(char)*(strlen("test it")+1));
	strcpy(myString,"test it");
	callBasicFuncs(myString,array_of_str,'A');
	free(myString);
	
	myString= malloc(sizeof(char)*(strlen("this is cat")+1));
	strcpy(myString,"this is cat");
	callBasicFuncs(myString,array_of_str,'A');
	free(myString);
	
	myString= malloc(sizeof(char)*(strlen("this is fox")+1));
	strcpy(myString,"this is fox");
	callBasicFuncs(myString,array_of_str,'A');
	free(myString);
	
	CuAssertIntEquals(tc,2,array_of_str->position);
	
	myString= malloc(sizeof(char)*(strlen("test it")+1));
	strcpy(myString,"test it");
	callBasicFuncs(myString,array_of_str,'D');		//will be deleted
	free(myString);
	
	CuAssertIntEquals(tc,1,array_of_str->position);
	CuAssertStrEquals(tc,"this",array_of_str->array[0].word); 	
	CuAssertStrEquals(tc,"is",array_of_str->array[0].nextWordArray->array[0].word); 	
	CuAssertIntEquals(tc,1,array_of_str->array[0].nextWordArray->position);
	CuAssertStrEquals(tc,"cat",array_of_str->array[0].nextWordArray->array[0].nextWordArray->array[0].word); 
	CuAssertStrEquals(tc,"fox",array_of_str->array[0].nextWordArray->array[0].nextWordArray->array[1].word); 
	CuAssertIntEquals(tc,2,array_of_str->array[0].nextWordArray->array[0].nextWordArray->position);
	
	
	
	myString= malloc(sizeof(char)*(strlen("test")+1));
	strcpy(myString,"test");
	callBasicFuncs(myString,array_of_str,'A');
	free(myString);
	
	myString= malloc(sizeof(char)*(strlen("test is")+1));
	strcpy(myString,"test is");
	callBasicFuncs(myString,array_of_str,'D');			//will not be deleted because full ngram doesn't exist 
	free(myString);
	


	CuAssertStrEquals(tc,"test",array_of_str->array[0].word); 	
	CuAssertStrEquals(tc,"this",array_of_str->array[1].word); 
	CuAssertIntEquals(tc,2,array_of_str->position);	
	CuAssertStrEquals(tc,"is",array_of_str->array[1].nextWordArray->array[0].word);
	CuAssertIntEquals(tc,1,array_of_str->array[1].nextWordArray->position);	
	CuAssertStrEquals(tc,"cat",array_of_str->array[1].nextWordArray->array[0].nextWordArray->array[0].word);
	CuAssertStrEquals(tc,"fox",array_of_str->array[1].nextWordArray->array[0].nextWordArray->array[1].word);
			
	
	
	myString= malloc(sizeof(char)*(strlen("this is")+1));
	strcpy(myString,"this is");
	callBasicFuncs(myString,array_of_str,'D');			//will not be deleted because this -> is -> cat and this -> is -> dog exist
	free(myString);
	
	
	
	CuAssertStrEquals(tc,"test",array_of_str->array[0].word); 	
	CuAssertStrEquals(tc,"this",array_of_str->array[1].word); 
	CuAssertIntEquals(tc,2,array_of_str->position);	
	CuAssertStrEquals(tc,"is",array_of_str->array[1].nextWordArray->array[0].word);
	CuAssertIntEquals(tc,1,array_of_str->array[1].nextWordArray->position);	
	CuAssertStrEquals(tc,"cat",array_of_str->array[1].nextWordArray->array[0].nextWordArray->array[0].word);
	CuAssertStrEquals(tc,"fox",array_of_str->array[1].nextWordArray->array[0].nextWordArray->array[1].word);
	CuAssertIntEquals(tc,2,array_of_str->array[1].nextWordArray->array[0].nextWordArray->position);		

	
	myString= malloc(sizeof(char)*(strlen("mouse")+1));
	strcpy(myString,"mouse");
	callBasicFuncs(myString,array_of_str,'D');			//will not be deleted because word does not exist
	free(myString);
	
	
	CuAssertStrEquals(tc,"test",array_of_str->array[0].word); 	
	CuAssertStrEquals(tc,"this",array_of_str->array[1].word); 
	CuAssertIntEquals(tc,2,array_of_str->position);	
	CuAssertStrEquals(tc,"is",array_of_str->array[1].nextWordArray->array[0].word);
	CuAssertIntEquals(tc,1,array_of_str->array[1].nextWordArray->position);	
	CuAssertStrEquals(tc,"cat",array_of_str->array[1].nextWordArray->array[0].nextWordArray->array[0].word);
	CuAssertStrEquals(tc,"fox",array_of_str->array[1].nextWordArray->array[0].nextWordArray->array[1].word);
	CuAssertIntEquals(tc,2,array_of_str->array[1].nextWordArray->array[0].nextWordArray->position);		
		
	
	
	deleteArray(array_of_str);
	array_of_str=NULL;
	CuAssertPtrEquals(tc,NULL,array_of_str);

	
}



CuSuite* FuncGetSuite() {		//adding TestFunc Functions into suite
    CuSuite* suite = CuSuiteNew();
    
    SUITE_ADD_TEST(suite, TestDelete_ngram);

    
    return suite;
}
