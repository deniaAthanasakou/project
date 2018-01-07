#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../compress.h"
#include "CuTestCompress.h"


void TestRecreateStructure_RecursiveCompression(CuTest *tc){
	HashTable *hashTable = createLinearHash(4,4);
	char *strings[4] = {"this","is","a","cat"};								//recreate whole
	int noOfWords = 4;
	insert_ngram(hashTable,strings,noOfWords,0);
	dataNode* lookUp = lookupTrieNode(strings[0],hashTable);
	CuAssertIntEquals(tc,0,lookUp->staticArrayLength);
	CuAssertPtrEquals(tc,NULL,lookUp->staticArray);
	
	arrayOfStructs* tempArray = lookUp->nextWordArray;
	for(int i=0; i<3;i++){
		CuAssertIntEquals(tc,0,tempArray->array[0].staticArrayLength);
		CuAssertPtrEquals(tc,NULL,tempArray->array[0].staticArray);
		tempArray = tempArray->array[0].nextWordArray;
	}
	
	recreateStructure(hashTable);
	
	lookUp = lookupTrieNode(strings[0],hashTable);
	CuAssertIntEquals(tc,3,lookUp->staticArrayLength);
	CuAssertPtrNotNull(tc,lookUp->staticArray);
	
	destroyLinearHash(hashTable);
	
	hashTable = createLinearHash(4,4);
	char *stringsNothing1[2] = {"this","is"};								//recreate nothing
	noOfWords = 2;
	insert_ngram(hashTable,stringsNothing1,noOfWords,0);
	char *stringsNothing2[2] = {"this","a"};								
	noOfWords = 2;
	insert_ngram(hashTable,stringsNothing2,noOfWords,0);
	
	lookUp = lookupTrieNode(strings[0],hashTable);
	CuAssertIntEquals(tc,0,lookUp->staticArrayLength);
	CuAssertPtrEquals(tc,NULL,lookUp->staticArray);
	
	tempArray = lookUp->nextWordArray;
	CuAssertIntEquals(tc,0,tempArray->array[0].staticArrayLength);
	CuAssertPtrEquals(tc,NULL,tempArray->array[0].staticArray);
	tempArray = tempArray->array[0].nextWordArray;
	
	recreateStructure(hashTable);
	
	lookUp = lookupTrieNode(strings[0],hashTable);
	CuAssertIntEquals(tc,0,lookUp->staticArrayLength);
	CuAssertPtrEquals(tc,NULL,lookUp->staticArray);
	
	
	tempArray = lookUp->nextWordArray;
	CuAssertIntEquals(tc,0,tempArray->array[0].staticArrayLength);
	CuAssertPtrEquals(tc,NULL,tempArray->array[0].staticArray);
	tempArray = tempArray->array[0].nextWordArray;
	
	destroyLinearHash(hashTable);

	hashTable = createLinearHash(4,4);
	char *stringsPartial1[3] = {"this","is","a"};								//recreate partial
	noOfWords = 3;
	insert_ngram(hashTable,stringsPartial1,noOfWords,0);
	char *stringsPartial2[2] = {"this","a"};								
	noOfWords = 2;
	insert_ngram(hashTable,stringsPartial2,noOfWords,0);
	
	lookUp = lookupTrieNode(strings[0],hashTable);
	CuAssertIntEquals(tc,0,lookUp->staticArrayLength);
	CuAssertPtrEquals(tc,NULL,lookUp->staticArray);
	
	tempArray = lookUp->nextWordArray;
	CuAssertIntEquals(tc,0,tempArray->array[0].staticArrayLength);
	CuAssertPtrEquals(tc,NULL,tempArray->array[0].staticArray);
	CuAssertIntEquals(tc,0,tempArray->array[1].staticArrayLength);
	CuAssertPtrEquals(tc,NULL,tempArray->array[1].staticArray);
	tempArray = tempArray->array[1].nextWordArray;
	CuAssertIntEquals(tc,0,tempArray->array[0].staticArrayLength);
	CuAssertPtrEquals(tc,NULL,tempArray->array[0].staticArray);
	
	recreateStructure(hashTable);
	
	lookUp = lookupTrieNode(strings[0],hashTable);
	CuAssertIntEquals(tc,0,lookUp->staticArrayLength);
	CuAssertPtrEquals(tc,NULL,lookUp->staticArray);
	
	tempArray = lookUp->nextWordArray;
	CuAssertIntEquals(tc,0,tempArray->array[0].staticArrayLength);
	CuAssertPtrEquals(tc,NULL,tempArray->array[0].staticArray);
	
	CuAssertIntEquals(tc,1,tempArray->array[1].staticArrayLength);
	CuAssertPtrNotNull(tc,tempArray->array[1].staticArray);
	
	destroyLinearHash(hashTable);	
	
}


void TestCheckForCompression(CuTest *tc){
	HashTable *hashTable = createLinearHash(4,4);
	char *strings[4] = {"this","is","a","cat"};								
	int noOfWords = 4;
	insert_ngram(hashTable,strings,noOfWords,0);
	dataNode* lookUp = lookupTrieNode(strings[0],hashTable);
	lookUp->staticArray = malloc(1*sizeof(int));
	CuAssertIntEquals(tc,0,lookUp->staticArrayLength);
	CuAssertStrEquals(tc,"this",lookUp->word);
	
	arrayOfStructs* tempArray = lookUp->nextWordArray;
	tempArray->array[0].staticArray = malloc(1*sizeof(int));
	CuAssertIntEquals(tc,0,tempArray->array[0].staticArrayLength);
	CuAssertStrEquals(tc,"is",tempArray->array[0].word);
	
	tempArray = tempArray->array[0].nextWordArray;
	tempArray->array[0].staticArray = malloc(1*sizeof(int));
	CuAssertIntEquals(tc,0,tempArray->array[0].staticArrayLength);
	CuAssertStrEquals(tc,"a",tempArray->array[0].word);
	
	tempArray = tempArray->array[0].nextWordArray;
	tempArray->array[0].staticArray = malloc(1*sizeof(int));
	CuAssertIntEquals(tc,0,tempArray->array[0].staticArrayLength);
	CuAssertStrEquals(tc,"cat",tempArray->array[0].word);
	
	
	int result = checkForCompression(lookUp);		//this is compressed
	CuAssertIntEquals(tc,1,result);
	
	result = checkForCompression(&lookUp->nextWordArray->array[0]);			//a cat	compressed	
	CuAssertIntEquals(tc,1,result);	

	
	destroyLinearHash(hashTable);
	
	hashTable = createLinearHash(4,4);
	char *strings1[2] = {"this","a"};								
	noOfWords = 2;
	insert_ngram(hashTable,strings1,noOfWords,0);
	char *strings2[2] = {"this","is"};								//recreate whole
	noOfWords = 2;
	
	insert_ngram(hashTable,strings2,noOfWords,0);
	lookUp = lookupTrieNode(strings1[0],hashTable);					//last one 
	result = checkForCompression(lookUp);
	CuAssertIntEquals(tc,0,result);
	
	destroyLinearHash(hashTable);
}

	
void TestCompress(CuTest *tc){
	dataNode* startNode = malloc(sizeof(dataNode));
	initializeDataNode(startNode);
	dataNode* additionalNode = malloc(sizeof(dataNode));
	initializeDataNode(additionalNode);

	insertString(startNode,"hello");						//two not null
	insertString(additionalNode,"world");
	
	CuAssertStrEquals(tc,"hello",startNode->word);
	CuAssertIntEquals(tc,0,startNode->staticArrayLength);
	CuAssertPtrEquals(tc,NULL,startNode->staticArray);
	CuAssertStrEquals(tc,"world",additionalNode->word);
	CuAssertIntEquals(tc,0,additionalNode->staticArrayLength);
	CuAssertPtrEquals(tc,NULL,additionalNode->staticArray);
	
	compress(startNode,additionalNode);
	
	
	CuAssertStrEquals(tc,"helloworld",startNode->dynamicWord);
	CuAssertStrEquals(tc,"world",additionalNode->word);
	
	CuAssertIntEquals(tc,0,startNode->staticArrayLength);
	CuAssertPtrNotNull(tc,startNode->staticArray);
	
	CuAssertIntEquals(tc,0,additionalNode->staticArrayLength);
	CuAssertPtrEquals(tc,NULL,additionalNode->staticArray);
	
	deleteDataNode(startNode);
	free(startNode);
	startNode=NULL;
	
	
	deleteDataNode(additionalNode);
	free(additionalNode);
	additionalNode=NULL;
	
}

void TestGetNgramFromNode(CuTest *tc){
	dataNode* temp1 = malloc(sizeof(dataNode));
	initializeDataNode(temp1);
	insertString(temp1,"hello");
	
	char** ngram1 = getNgramFromNode(temp1);
	CuAssertStrEquals(tc,"hello",ngram1[0]);
	
	dataNode* temp2 = malloc(sizeof(dataNode));
	initializeDataNode(temp2);
	temp2->dynamicWord = malloc((strlen("helloworlds")+1)*sizeof(char));
	strcpy(temp2->dynamicWord,"helloworlds");

	temp2->staticArray = malloc(2*sizeof(int));
	temp2->staticArray[0] = -5;
	temp2->staticArray[1] = 6;
	temp2->staticArrayLength = 1;
	char** ngram2 = getNgramFromNode(temp2);			
	CuAssertStrEquals(tc,"hello",ngram2[0]);
	CuAssertStrEquals(tc,"worlds",ngram2[1]);
	
	
	free(ngram1[0]);
	ngram1[0] = NULL;
	free(ngram1);
	ngram1 = NULL;
	deleteDataNode(temp1);
	free(temp1);
	temp1=NULL;
	
	
	free(ngram2[0]);
	ngram2[0] = NULL;
	
	free(ngram2[1]);
	ngram2[1] = NULL;
	
	free(ngram2);
	ngram2 = NULL;
	deleteDataNode(temp2);
	free(temp2);
	temp2=NULL;
}



CuSuite* CompressGetSuite() {				//adding TestCompress Functions into suite
    CuSuite* suite = CuSuiteNew();
    
    SUITE_ADD_TEST(suite, TestRecreateStructure_RecursiveCompression);
	SUITE_ADD_TEST(suite, TestCheckForCompression);
	SUITE_ADD_TEST(suite, TestCompress);
	SUITE_ADD_TEST(suite, TestGetNgramFromNode);
	
	
    return suite;
}

