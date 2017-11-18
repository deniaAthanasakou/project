#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "CuTestAuxMethods.h"


void TestDeleteArrayOfWords(CuTest *tc) {
    char** array=NULL;
	int length=0;									//delete null array
	deleteArrayOfWords(array, length);
	free(array);
	array = NULL;
	CuAssertPtrEquals(tc,NULL,array); 
	
	length=1;										//delete array with one element
	char** array2 = malloc(sizeof(char*)*length);
	array2[0]= malloc(sizeof(char)*(strlen("Hello")+1));
	strcpy(array2[0],"Hello");
	deleteArrayOfWords(array2, length);
	free(array2);
	array2 = NULL;
	CuAssertPtrEquals(tc,NULL,array2); 
	
	length=4;										//delete array with multiple elements
	char** array3 = malloc(sizeof(char*)*length);
	
	array3[0]= malloc(sizeof(char)*(strlen("The")+1));
	strcpy(array3[0],"The");
	array3[1]= malloc(sizeof(char)*(strlen("fox,")+1));
	strcpy(array3[1],"fox,");
	array3[2]= malloc(sizeof(char)*(strlen("the")+1));
	strcpy(array3[2],"the");
	array3[3]= malloc(sizeof(char)*(strlen("cat.")+1));
	strcpy(array3[3],"cat.");
	
	deleteArrayOfWords(array3, length);
	free(array3);
	array3 = NULL;
	CuAssertPtrEquals(tc,NULL,array3); 

}


void TestStringToArray(CuTest *tc){
	char* ngram=NULL;
	arrayWords* array = stringToArray(ngram);
	
	CuAssertPtrNotNull(tc,array);
	CuAssertIntEquals(tc,0,array->length);    
	CuAssertPtrNotNull(tc,array->words);
	 
	deleteArrayOfWords(array->words, array->length);
	free(array->words);
	array->words = NULL;
	free(array);
	array=NULL;
	
	ngram=malloc((strlen("The fox is bad and the cat is good.")+1) * sizeof(char));
	strcpy(ngram,"The fox is bad and the cat is good.");
	array = stringToArray(ngram);
	
	CuAssertPtrNotNull(tc,array);
	CuAssertIntEquals(tc,9,array->length);    
	CuAssertPtrNotNull(tc,array->words);
	
	CuAssertStrEquals(tc,"The",array->words[0]); 
	CuAssertStrEquals(tc,"fox",array->words[1]); 
	CuAssertStrEquals(tc,"is",array->words[2]); 
	CuAssertStrEquals(tc,"bad",array->words[3]); 
	CuAssertStrEquals(tc,"and",array->words[4]); 
	CuAssertStrEquals(tc,"the",array->words[5]); 
	CuAssertStrEquals(tc,"cat",array->words[6]); 
	CuAssertStrEquals(tc,"is",array->words[7]); 
	CuAssertStrEquals(tc,"good.",array->words[8]); 
	
	
	deleteArrayOfWords(array->words, array->length);
	free(array->words);
	array->words = NULL;
	free(array);
	array=NULL;
	
	free(ngram);
	ngram=NULL;
}


void TestInitialize(CuTest *tc){
	/*text inside test_initialize:
		test
		hi
		this dog
		this cat
	*/

	arrayOfStructs* structureTree = (arrayOfStructs*) malloc(1 * sizeof(arrayOfStructs));
	initializeArray(structureTree);
	FILE * initFile;
	initFile = fopen ("testingFiles/test_initialize","r");
	if (initFile!=NULL)
	{
		initialize(initFile, structureTree);
		fclose (initFile);
	}
	
	CuAssertIntEquals(tc,10,structureTree->length);  
	CuAssertIntEquals(tc,3,structureTree->position);      
	CuAssertPtrNotNull(tc,structureTree->array);
	
	CuAssertStrEquals(tc,"hi",structureTree->array[0].word); 
	CuAssertPtrEquals(tc,NULL,structureTree->array[0].nextWordArray);
	CuAssertTrue(tc, structureTree->array[0].isFinal);
	
	CuAssertStrEquals(tc,"test",structureTree->array[1].word); 
	CuAssertPtrEquals(tc,NULL,structureTree->array[1].nextWordArray);
	CuAssertTrue(tc, structureTree->array[1].isFinal);
	
	CuAssertStrEquals(tc,"this",structureTree->array[2].word); 
	CuAssertPtrNotNull(tc,structureTree->array[2].nextWordArray);
	CuAssertTrue(tc, !structureTree->array[2].isFinal);
	
	
	arrayOfStructs* nextArray = structureTree->array[2].nextWordArray; 
	
	CuAssertIntEquals(tc,10,nextArray->length);  
	CuAssertIntEquals(tc,2,nextArray->position);      
	CuAssertPtrNotNull(tc,nextArray->array);
	
	CuAssertStrEquals(tc,"cat",nextArray->array[0].word); 
	CuAssertPtrEquals(tc,NULL,nextArray->array[0].nextWordArray);
	CuAssertTrue(tc, nextArray->array[0].isFinal);
	
	CuAssertStrEquals(tc,"dog",nextArray->array[1].word); 
	CuAssertPtrEquals(tc,NULL,nextArray->array[1].nextWordArray);
	CuAssertTrue(tc, nextArray->array[1].isFinal);
	
	
	deleteArray(structureTree);
	structureTree=NULL;
	CuAssertPtrEquals(tc,NULL,structureTree);
}

void TestBinarySearch(CuTest* tc){

	arrayOfStructs* array_of_str = malloc(sizeof(arrayOfStructs));
	initializeArray(array_of_str);
	checkItemExists* getPosition;
	
	dataNode *item = malloc(sizeof(dataNode));
	item->word = malloc(10 * sizeof(char));
	getPosition = binarySearch(array_of_str,item,0,array_of_str->position,NULL);			//array is empty
	CuAssertIntEquals(tc,-1,getPosition->position);
	CuAssertTrue(tc,!getPosition->exists);
	
	free(getPosition);
	getPosition = NULL;
	
		
	strcpy(item->word,"cat");										//existing word
	array_of_str->array[0].word = malloc(10*sizeof(char));
	strcpy(array_of_str->array[0].word,"cat");
	array_of_str->position++;
	array_of_str->array[1].word = malloc(10*sizeof(char));
	strcpy(array_of_str->array[1].word,"dog");
	array_of_str->position++;
	
	getPosition = binarySearch(array_of_str,item,0,array_of_str->position,NULL);
	CuAssertIntEquals(tc,0,getPosition->position);
	CuAssertTrue(tc,getPosition->exists);
		
	free(getPosition);
	getPosition = NULL;	
		
	strcpy(item->word,"hello");																//non existing word
	getPosition = binarySearch(array_of_str,item,0,array_of_str->position,NULL);
	CuAssertIntEquals(tc,-1,getPosition->position);
	CuAssertTrue(tc,!getPosition->exists);
	
	free(getPosition);
	getPosition = NULL;
	
	deleteDataNode(item);
	free(item);
	item = NULL;
	deleteArray(array_of_str);
	array_of_str = NULL;
	
}

void TestInsertionSort(CuTest* tc){

	arrayOfStructs* array_of_str = malloc(sizeof(arrayOfStructs));
	initializeArray(array_of_str);

	dataNode *item = malloc(sizeof(dataNode));
	item->word=NULL;
	checkItemExists* getPosition = insertionSort(array_of_str,item,array_of_str->position);   //array is empty  
	CuAssertIntEquals(tc,0,getPosition->position);
	CuAssertTrue(tc,!getPosition->exists);
	
	free(getPosition);
	getPosition = NULL;

	dataNode *item1 = malloc(sizeof(dataNode));
	item1->word = malloc((strlen("cat")+1) * sizeof(char));
	strcpy(item1->word,"cat");     
	item1->nextWordArray=NULL;
	array_of_str->array[0]= *item1;
	array_of_str->position++;
	
	
	dataNode *item2 = malloc(sizeof(dataNode));
	item2->word = malloc((strlen("dog")+1) * sizeof(char));
	strcpy(item2->word,"dog");     
	item2->nextWordArray=NULL;
	array_of_str->array[1]= *item2;
	array_of_str->position++;
	
	
	getPosition = insertionSort(array_of_str,item1,array_of_str->position);		//try to insert cat
	CuAssertTrue(tc,getPosition->position!=-1);
	CuAssertTrue(tc,getPosition->exists);
	
	free(getPosition);
	getPosition = NULL;
	
	
	dataNode *item3 = malloc(sizeof(dataNode));
	item3->word = malloc((strlen("hello")+1) * sizeof(char));
	strcpy(item3->word,"hello");     
	item3->nextWordArray=NULL;
	
	getPosition = insertionSort(array_of_str,item3,array_of_str->position);
	array_of_str->position++;               //non existing word
	CuAssertIntEquals(tc,2,getPosition->position);
	CuAssertTrue(tc,!getPosition->exists);
	
	free(getPosition);
	getPosition = NULL;
	
	
	dataNode *item4 = malloc(sizeof(dataNode));
	item4->word = malloc((strlen("ant")+1) * sizeof(char));
	strcpy(item4->word,"ant");     
	item4->nextWordArray=NULL;
	
	getPosition = insertionSort(array_of_str,item4,array_of_str->position);
	array_of_str->position++;               	//non existing word

	CuAssertIntEquals(tc,0,getPosition->position);
	CuAssertTrue(tc,!getPosition->exists);
	CuAssertStrEquals(tc,"ant",array_of_str->array[0].word);
	CuAssertStrEquals(tc,"cat",array_of_str->array[1].word);
	CuAssertStrEquals(tc,"dog",array_of_str->array[2].word);
	CuAssertStrEquals(tc,"hello",array_of_str->array[3].word);
	
	free(getPosition);
	getPosition = NULL;
	
	int lengthBefore = array_of_str->length;
	
	
	//add in order to double array
	//5th
	dataNode *item5 = malloc(sizeof(dataNode));
	item5->word = malloc((strlen("hi")+1) * sizeof(char));
	strcpy(item5->word,"hi");     
	item5->nextWordArray=NULL;
	
	getPosition = insertionSort(array_of_str,item5,array_of_str->position);
	array_of_str->position++;
	
	free(getPosition);
	getPosition = NULL;
	
	//6th
	dataNode *item6 = malloc(sizeof(dataNode));
	item6->word = malloc((strlen("testing")+1) * sizeof(char));
	strcpy(item6->word,"testing");     
	item6->nextWordArray=NULL;
	
	getPosition = insertionSort(array_of_str,item6,array_of_str->position);
	array_of_str->position++;
	
	free(getPosition);
	getPosition = NULL;
	
	//7th
	dataNode *item7 = malloc(sizeof(dataNode));
	item7->word = malloc((strlen("before")+1) * sizeof(char));
	strcpy(item7->word,"before");     
	item7->nextWordArray=NULL;
	
	getPosition = insertionSort(array_of_str,item7,array_of_str->position);
	array_of_str->position++;
	
	free(getPosition);
	getPosition = NULL;
	
	//8th
	dataNode *item8 = malloc(sizeof(dataNode));
	item8->word = malloc((strlen("those")+1) * sizeof(char));
	strcpy(item8->word,"those");     
	item8->nextWordArray=NULL;
	
	getPosition = insertionSort(array_of_str,item8,array_of_str->position);
	array_of_str->position++;
	
	free(getPosition);
	getPosition = NULL;
	
	//9th
	dataNode *item9 = malloc(sizeof(dataNode));
	item9->word = malloc((strlen("what")+1) * sizeof(char));
	strcpy(item9->word,"what");     
	item9->nextWordArray=NULL;
	
	getPosition = insertionSort(array_of_str,item9,array_of_str->position);
	array_of_str->position++;
	
	free(getPosition);
	getPosition = NULL;
	
	//10th
	dataNode *item10 = malloc(sizeof(dataNode));
	item10->word = malloc((strlen("myy")+1) * sizeof(char));
	strcpy(item10->word,"myy");     
	item10->nextWordArray=NULL;
	
	getPosition = insertionSort(array_of_str,item10,array_of_str->position);
	array_of_str->position++;
	
	free(getPosition);
	getPosition = NULL;
	
	//check if array has been doubled
	
	CuAssertIntEquals(tc,2*lengthBefore,array_of_str->length);
	
	deleteDataNode(item);
	free(item);
	item=NULL;
	
	
	deleteDataNode(item1);
	free(item1);
	item1=NULL;
	
	
	deleteDataNode(item2);
	free(item2);
	item2=NULL;
	
	deleteDataNode(item3);
	free(item3);
	item3=NULL;
	
	deleteDataNode(item4);
	free(item4);
	item4=NULL;
	
	deleteDataNode(item5);
	free(item5);
	item5=NULL;
	
	deleteDataNode(item6);
	free(item6);
	item6=NULL;
	
	deleteDataNode(item7);
	free(item7);
	item7=NULL;
	
	deleteDataNode(item8);
	free(item8);
	item8=NULL;
	
	deleteDataNode(item9);
	free(item9);
	item9=NULL;
	
	deleteDataNode(item10);
	free(item10);
	item10=NULL;
	
	
	free(array_of_str->array);
	array_of_str->array = NULL;
	free(array_of_str);
	array_of_str = NULL;
	

}

void TestExecuteQueryFile(CuTest *tc){
	
	
	printf("Start of Testing executeQueryFile\n");
	FILE * queryFile;
	queryFile = fopen ("testingFiles/correct1batch","r");
	arrayOfStructs* array_of_str = malloc(sizeof(arrayOfStructs));
	initializeArray(array_of_str);
	
	CuAssertIntEquals(tc,1,executeQueryFile(queryFile,array_of_str));		//correct file
	
	fclose (queryFile);
	array_of_str = NULL;
	CuAssertPtrEquals(tc,NULL,array_of_str);
	
	queryFile = fopen ("testingFiles/correctMultipleBatches","r");
	array_of_str = malloc(sizeof(arrayOfStructs));
	initializeArray(array_of_str);
	CuAssertIntEquals(tc,1,executeQueryFile(queryFile,array_of_str));					//correct file with multiple batches
	fclose (queryFile);
	array_of_str = NULL;
	CuAssertPtrEquals(tc,NULL,array_of_str);
	
	queryFile = fopen ("testingFiles/incorrectNoF","r");
	array_of_str = malloc(sizeof(arrayOfStructs));
	initializeArray(array_of_str);
	CuAssertIntEquals(tc,0,executeQueryFile(queryFile,array_of_str));					//file does not end with F
	fclose (queryFile);
	array_of_str = NULL;
	CuAssertPtrEquals(tc,NULL,array_of_str);
	
	queryFile = fopen ("testingFiles/IncorrectLetter","r");
	array_of_str = malloc(sizeof(arrayOfStructs));
	initializeArray(array_of_str);
	CuAssertIntEquals(tc,0,executeQueryFile(queryFile,array_of_str));					//query of file starts with sth else other than A,D,Q,F
	fclose (queryFile);
	array_of_str = NULL;
	CuAssertPtrEquals(tc,NULL,array_of_str);
	
	printf("End of Testing executeQueryFile\n");

}

void TestCheckIfStringExists(CuTest *tc){

	int noOfWords=0;
	char** array=NULL;
	char* str = malloc((strlen("")+1)* sizeof(char));
	CuAssertIntEquals(tc,0,checkIfStringExists(array, noOfWords, str));			//checking null array
	
	noOfWords=0;
	array=malloc(noOfWords * sizeof(char*));
	str = realloc(str,(strlen("")+1)* sizeof(char));
	strcpy(str,"");
	CuAssertIntEquals(tc,0,checkIfStringExists(array, noOfWords, str));				//checking array with no elements
	
	noOfWords=4;
	array=realloc(array,noOfWords * sizeof(char*));
	array[0]=malloc((strlen("cat")+1) * sizeof(char));
	strcpy(array[0],"cat");
	array[1]=malloc((strlen("dog")+1) * sizeof(char));
	strcpy(array[1],"dog");
	array[2]=malloc((strlen("mouse")+1) * sizeof(char));
	strcpy(array[2],"mouse");
	array[3]=malloc((strlen("fox")+1) * sizeof(char));
	strcpy(array[3],"fox");
	
	str = realloc(str,(strlen("pigeon")+1)* sizeof(char));
	strcpy(str,"pigeon");
	CuAssertIntEquals(tc,0,checkIfStringExists(array, noOfWords, str));			//checking array with word that does not exist inside it
	
	str = realloc(str,(strlen("dog")+1)* sizeof(char));
	strcpy(str,"dog");
	CuAssertIntEquals(tc,1,checkIfStringExists(array, noOfWords, str));				//checking array with word that does exist inside it
	
	free(str);
	str=NULL;
	
	for(int i=0; i<noOfWords; i++){
		free(array[i]);
		array[i]=NULL;
	}
	free(array);
	array=NULL;

}

void TestDeletionSort(CuTest *tc){

	arrayOfStructs* array_of_str = malloc(sizeof(arrayOfStructs));
	initializeArray(array_of_str);
	
	char** item = malloc(1*sizeof(char*));
	item[0] = malloc((strlen("hello")+1) * sizeof(char));
	strcpy(item[0],"hello");																
	insert_ngram(array_of_str, item, 1);							//add 'hello'
	

	deletionSort(array_of_str,0,array_of_str->position);		//delete only element
	CuAssertPtrNotNull(tc,array_of_str);
	CuAssertPtrNotNull(tc,array_of_str->array);
	CuAssertIntEquals(tc,10,array_of_str->length);	
	CuAssertIntEquals(tc,0,array_of_str->position);	
	
	free(item[0]);
	item[0]=NULL;
	free(item);
	item=NULL;
	
	
	item = malloc(1*sizeof(char*));
	
	item[0] = malloc((strlen("hello")+1) * sizeof(char));
	strcpy(item[0],"hello");																
	insert_ngram(array_of_str, item, 1);							//add 'hello'
	
	free(item[0]);
	item[0]=NULL;
	free(item);
	item=NULL;
	
	item = malloc(1*sizeof(char*));
	item[0] = malloc((strlen("test")+1) * sizeof(char));
	strcpy(item[0],"test");																
	insert_ngram(array_of_str, item, 1);							//add 'test'
	
	free(item[0]);
	item[0]=NULL;
	free(item);
	item=NULL;
	
	item = malloc(1*sizeof(char*));
	item[0] = malloc((strlen("the")+1) * sizeof(char));
	strcpy(item[0],"the");																
	insert_ngram(array_of_str, item, 1);							//add 'the'
	
	free(item[0]);
	item[0]=NULL;
	free(item);
	item=NULL;
	
	item = malloc(1*sizeof(char*));
	item[0] = malloc((strlen("this")+1) * sizeof(char));
	strcpy(item[0],"this");																
	insert_ngram(array_of_str, item, 1);							//add 'this'
	
	free(item[0]);
	item[0]=NULL;
	free(item);
	item=NULL;
	
	deletionSort(array_of_str,3,array_of_str->position);		//from hello, test, the, this delete this
	
	CuAssertPtrNotNull(tc,array_of_str);
	CuAssertPtrNotNull(tc,array_of_str->array);
	CuAssertIntEquals(tc,10,array_of_str->length);	
	CuAssertIntEquals(tc,3,array_of_str->position);	
	
	CuAssertStrEquals(tc,"hello",array_of_str->array[0].word); 
	CuAssertStrEquals(tc,"test",array_of_str->array[1].word); 
	CuAssertStrEquals(tc,"the",array_of_str->array[2].word); 
	
	item = malloc(1*sizeof(char*));
	item[0] = malloc((strlen("this")+1) * sizeof(char));
	strcpy(item[0],"this");																
	insert_ngram(array_of_str, item, 1);							//add 'this'
	
	free(item[0]);
	item[0]=NULL;
	free(item);
	item=NULL;
	
	deletionSort(array_of_str,1,array_of_str->position);		//from hello, test, the, this delete test
	CuAssertPtrNotNull(tc,array_of_str);
	CuAssertPtrNotNull(tc,array_of_str->array);
	CuAssertIntEquals(tc,10,array_of_str->length);	
	CuAssertIntEquals(tc,3,array_of_str->position);	
	
	CuAssertStrEquals(tc,"hello",array_of_str->array[0].word); 
	CuAssertStrEquals(tc,"the",array_of_str->array[1].word); 
	CuAssertStrEquals(tc,"this",array_of_str->array[2].word); 
	
	
	
	deleteArray(array_of_str);
	array_of_str=NULL;
}

void TestCallBasicFuncs(CuTest *tc){
 
 	printf("Start testing callBasicFunctions\n");
 	
	arrayOfStructs* array_of_str = malloc(sizeof(arrayOfStructs));
	initializeArray(array_of_str);
	char* ngram = malloc(20*sizeof(char));
	strcpy(ngram,"A cat in my shoe");
	
	callBasicFuncs(ngram, array_of_str, 'A');
	free(ngram);
	ngram = NULL;
	
	ngram = malloc(20*sizeof(char));
	strcpy(ngram,"A cat in my shoe");
	callBasicFuncs(ngram, array_of_str, 'Q');
	free(ngram);
	ngram = NULL;
	
	ngram = malloc(20*sizeof(char));
	strcpy(ngram,"A cat in my shoe"); 
	callBasicFuncs(ngram, array_of_str, 'D');
	free(ngram);
	ngram = NULL;
	
	deleteArray(array_of_str);
	array_of_str = NULL;
	
	printf("End of testing callBasicFunctions\n");
}


CuSuite* AuxMethodsGetSuite() {		//adding TestAuxMethods Functions into suite
    CuSuite* suite = CuSuiteNew();
    
    SUITE_ADD_TEST(suite, TestDeleteArrayOfWords);
    SUITE_ADD_TEST(suite, TestStringToArray);
    SUITE_ADD_TEST(suite, TestInitialize);
	SUITE_ADD_TEST(suite, TestBinarySearch);
	SUITE_ADD_TEST(suite, TestInsertionSort);
    SUITE_ADD_TEST(suite, TestExecuteQueryFile);
    SUITE_ADD_TEST(suite, TestCheckIfStringExists);
    SUITE_ADD_TEST(suite, TestDeletionSort);
	SUITE_ADD_TEST(suite, TestCallBasicFuncs);
    
    return suite;
}

