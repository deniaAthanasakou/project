#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../compress.h" 
#include "CuTestFunc.h"


void TestInsert_ngram(CuTest *tc){
	
	HashTable* hashTable = createLinearHash(NOOFBUCKETS, NOOFCELLS);
	arrayOfStructs* tempArray = NULL;
	
	char** arrayOfWords = malloc(12*sizeof(char*));
	int noOfWords;
	
	
	arrayOfWords[0] = malloc(10* sizeof(char));
	arrayOfWords[1] = malloc(10* sizeof(char));
	arrayOfWords[2] = malloc(40* sizeof(char));
	arrayOfWords[3] = malloc(10* sizeof(char));
	arrayOfWords[4] = malloc(10* sizeof(char));
	arrayOfWords[5] = malloc(10* sizeof(char));
	arrayOfWords[6] = malloc(10* sizeof(char));
	arrayOfWords[7] = malloc(10* sizeof(char));
	arrayOfWords[8] = malloc(10* sizeof(char));
	arrayOfWords[9] = malloc(10* sizeof(char));
	arrayOfWords[10] = malloc(10* sizeof(char));
	arrayOfWords[11] = malloc(10* sizeof(char));
	
	noOfWords = 3;
	strcpy(arrayOfWords[0],"this");										//check if nextWordArray is created -> insert phrase 'this dog "big phrase"'
	strcpy(arrayOfWords[1],"dog");
	strcpy(arrayOfWords[2],"whatagreatworldthatisiamflattered");
	insert_ngram(hashTable, arrayOfWords, noOfWords, 0);
	CuAssertStrEquals(tc,"this",hashTable->buckets[0].cells[0].word);
	CuAssertPtrEquals(tc,NULL,hashTable->buckets[0].cells[0].dynamicWord);
	CuAssertIntEquals(tc,strlen("this")+1,hashTable->buckets[0].cells[0].noOfChars);
	CuAssertTrue(tc,!hashTable->buckets[0].cells[0].isDynamic);
	CuAssertTrue(tc,!hashTable->buckets[0].cells[0].isFinal);
	CuAssertIntEquals(tc,0,hashTable->buckets[0].cells[0].additionVersion);
	CuAssertIntEquals(tc,-1,hashTable->buckets[0].cells[0].deletionVersion);
	CuAssertIntEquals(tc,-1,hashTable->buckets[0].cells[0].notFinalInDeletionVersion);
	CuAssertIntEquals(tc,-1,hashTable->buckets[0].cells[0].finalSinceAdditionVersion);
	
	tempArray = hashTable->buckets[0].cells[0].nextWordArray;
	CuAssertStrEquals(tc,"dog",tempArray->array[0].word);
	CuAssertTrue(tc,!tempArray->array[0].isFinal);
	CuAssertIntEquals(tc,0,tempArray->array[0].additionVersion);
	CuAssertIntEquals(tc,-1,tempArray->array[0].deletionVersion);
	CuAssertIntEquals(tc,-1,tempArray->array[0].notFinalInDeletionVersion);
	CuAssertIntEquals(tc,-1,tempArray->array[0].finalSinceAdditionVersion);
	
	tempArray = hashTable->buckets[0].cells[0].nextWordArray->array[0].nextWordArray;					//check for big word insertion
	CuAssertStrEquals(tc,"\0",tempArray->array[0].word);
	CuAssertStrEquals(tc,"whatagreatworldthatisiamflattered",tempArray->array[0].dynamicWord);
	CuAssertIntEquals(tc,strlen(tempArray->array[0].dynamicWord)+1,tempArray->array[0].noOfChars);
	CuAssertTrue(tc,tempArray->array[0].isDynamic);
	CuAssertTrue(tc,tempArray->array[0].isFinal);
	CuAssertIntEquals(tc,0,tempArray->array[0].additionVersion);
	CuAssertIntEquals(tc,-1,tempArray->array[0].deletionVersion);
	CuAssertIntEquals(tc,-1,tempArray->array[0].notFinalInDeletionVersion);
	CuAssertIntEquals(tc,0,tempArray->array[0].finalSinceAdditionVersion);
	
	
	
	strcpy(arrayOfWords[0],"hello");															//check if insert is sorting array -> insert phrase 'hello dog'
	strcpy(arrayOfWords[1],"dog");
	noOfWords = 2;
	insert_ngram(hashTable, arrayOfWords, noOfWords,1);
	CuAssertStrEquals(tc,"hello", hashTable->buckets[0].cells[0].word);
	CuAssertStrEquals(tc,"this", hashTable->buckets[0].cells[1].word);
	CuAssertTrue(tc,! hashTable->buckets[0].cells[0].isFinal);
	CuAssertTrue(tc,! hashTable->buckets[0].cells[1].isFinal);
	tempArray = hashTable->buckets[0].cells[0].nextWordArray;
	CuAssertStrEquals(tc,"dog",tempArray->array[0].word);
	CuAssertTrue(tc,tempArray->array[0].isFinal);
	
	strcpy(arrayOfWords[0],"hello");															//check if insert is sorting nextWordArray -> insert phrase 'hello hello'
	strcpy(arrayOfWords[1],"hello");
	noOfWords = 2;
	insert_ngram(hashTable, arrayOfWords, noOfWords,2);
	CuAssertStrEquals(tc,"hello",hashTable->buckets[0].cells[0].word);
	CuAssertStrEquals(tc,"this",hashTable->buckets[0].cells[1].word);
	CuAssertTrue(tc,!hashTable->buckets[0].cells[0].isFinal);
	CuAssertTrue(tc,!hashTable->buckets[0].cells[1].isFinal);
	tempArray = hashTable->buckets[0].cells[0].nextWordArray;
	CuAssertStrEquals(tc,"dog",tempArray->array[0].word);
	CuAssertStrEquals(tc,"hello",tempArray->array[1].word);
	CuAssertTrue(tc,tempArray->array[0].isFinal);
	
	char* temp = malloc(10*sizeof(char));
	char* number = malloc(3*sizeof(char));
	
	noOfWords = 2;
	strcpy(arrayOfWords[0],"hello");	
	for(int i=0;i<12;i++){
		sprintf(number,"%d",i);	
		strcpy(temp,"hello");															//check double of array
		strcat(temp,number);
		strcpy(arrayOfWords[1],temp);										
		insert_ngram(hashTable, arrayOfWords, noOfWords,3);
	}
	CuAssertIntEquals(tc,20,hashTable->buckets[0].cells[0].nextWordArray->length);
	
	for(int i=0;i<12;i++){												//free 
		free(arrayOfWords[i]);
		arrayOfWords[i] = NULL;
	}
	free(arrayOfWords);
	arrayOfWords = NULL;
	free(temp);
	temp = NULL;
	free(number);
	number = NULL;
	destroyLinearHash(hashTable);
	hashTable = NULL;
}


void TestSearch_ngram(CuTest *tc){
	
	HashTable* hashTable = createLinearHash(NOOFBUCKETS, NOOFCELLS);
	
	printf("Start of testing search_ngram\n");
	arrayOfStructs* tempArray = NULL;
	BloomFilter* topFilter = NULL;
	topKArray* topArray = NULL;
	
	char** arrayOfWords = malloc(3*sizeof(char*));
	int noOfWords;
	
	arrayOfWords[0] = malloc(10* sizeof(char));
	arrayOfWords[1] = malloc(40* sizeof(char));
	arrayOfWords[2] = malloc(10* sizeof(char));
	
	
	noOfWords = 3;
	strcpy(arrayOfWords[0],"cat");										//get search phrase ->found
	strcpy(arrayOfWords[1],"dogisthebestpetexceptforcats");
	strcpy(arrayOfWords[2],"record");

	insert_ngram(hashTable, arrayOfWords, noOfWords, 1);
	
	topFilter = initializeFilter(5);		//initialize bloomFilter here
	topArray = initializeTopKArray();		//initialize topKArray
	
	char* searchString;
	int* noOfWordsPtr = &noOfWords;
	int instrNum = 2;
	int* instrNumPtr = &instrNum;
	void** p1 = malloc(6*sizeof(void*));
	p1[0] = (void*) hashTable;
	p1[1] = (void*) arrayOfWords;
	p1[2] = (void*) noOfWordsPtr;
	p1[3] = (void*) topFilter;
	p1[4] = (void*) topArray;
	p1[5] = (void*) instrNumPtr;
	searchString = search_ngram(p1);		//query after insert
	
	CuAssertStrEquals(tc,"cat dogisthebestpetexceptforcats record",searchString);
	free(searchString);
	searchString=NULL;
	
	
	instrNum = 0;
	instrNumPtr = &instrNum;
	p1[5] = (void*) instrNumPtr;
	searchString = search_ngram(p1);		//query before insert
	
	CuAssertStrEquals(tc,"-1",searchString);
	free(searchString);
	searchString=NULL;
	
	for(int i=0;i<noOfWords;i++){												//free 
		free(arrayOfWords[i]);
		arrayOfWords[i] = NULL;
	}
	
	*instrNumPtr = 1;
	*noOfWordsPtr = 1;
	noOfWords = 1;
	arrayOfWords[0] = malloc(10* sizeof(char));														//not found
	strcpy(arrayOfWords[0],"init");
	p1[1] = (void*) arrayOfWords;
	searchString = search_ngram(p1);
	CuAssertStrEquals(tc,"-1",searchString);
	free(searchString);
	searchString=NULL;
	
												
	free(arrayOfWords[0]);					//free 
	arrayOfWords[0] = NULL;

	arrayOfWords[0] = malloc(10* sizeof(char));
	*instrNumPtr = 3;
	*noOfWordsPtr = 1;
	noOfWords = 1;
	strcpy(arrayOfWords[0],"cat");										//get search phrase ->found
	insert_ngram(hashTable, arrayOfWords, 1, 2);
	p1[1] = (void*) arrayOfWords;
	searchString = search_ngram(p1);
	CuAssertStrEquals(tc,"cat",searchString);
	free(searchString);
	searchString=NULL;
	
	free(arrayOfWords[0]);
	arrayOfWords[0] = NULL;
	
	
	arrayOfWords[0] = malloc(40* sizeof(char));
	strcpy(arrayOfWords[0],"heterotransplantations");
	insert_ngram(hashTable, arrayOfWords, 1, 4);
	free(arrayOfWords[0]);
	arrayOfWords[0] = NULL;
	
	arrayOfWords[0] = malloc(10* sizeof(char));
	strcpy(arrayOfWords[0],"this");
	insert_ngram(hashTable, arrayOfWords, 1, 5);
	free(arrayOfWords[0]);
	arrayOfWords[0] = NULL;
	
	noOfWords = 3;
	char** temp = malloc(3*sizeof(char*));
	temp[0] =  malloc(10* sizeof(char));
	temp[1] =  malloc(10* sizeof(char));
	temp[2] =  malloc(40* sizeof(char));
	strcpy(temp[0],"cat");
	strcpy(temp[1],"is");
	strcpy(temp[2],"heterotransplantations");
	p1[1] = (void*) temp;
	*instrNumPtr = 6;
	*noOfWordsPtr = 3;
	searchString = search_ngram(p1);
	
	//found multiple ones
	CuAssertStrEquals(tc,"catheterotransplantations",searchString);
	free(searchString);
	searchString=NULL;
	

	
	for(int i=0;i<3;i++){
		free(temp[i]);												//free 
		temp[i] = NULL;
	}
	
	
	free(temp);
	temp = NULL;
	free(arrayOfWords);
	arrayOfWords = NULL;
	

	//check query results were inserted into topArray
	CuAssertIntEquals(tc,20, topArray->length);
	CuAssertIntEquals(tc,3, topArray->positionInsertion);
	
	CuAssertStrEquals(tc,"cat",topArray->array[0].ngram);
	CuAssertIntEquals(tc,2,topArray->array[0].occurences);
	
	CuAssertStrEquals(tc,"cat dogisthebestpetexceptforcats record",topArray->array[1].ngram);
	CuAssertIntEquals(tc,1,topArray->array[1].occurences);
	
	CuAssertStrEquals(tc,"heterotransplantations",topArray->array[2].ngram);
	CuAssertIntEquals(tc,1,topArray->array[2].occurences);
	
	
	arrayOfWords = malloc(3*sizeof(char*));
	arrayOfWords[0] = malloc(10* sizeof(char));
	arrayOfWords[1] = malloc(10* sizeof(char));
	arrayOfWords[2] = malloc(10* sizeof(char));
	
	noOfWords = 3;
	strcpy(arrayOfWords[0],"hello");										//get search phrase ->found
	strcpy(arrayOfWords[1],"world");
	strcpy(arrayOfWords[2],"!");
	
	insert_ngram(hashTable, arrayOfWords, noOfWords, 7);
	delete_ngram(hashTable, arrayOfWords, noOfWords, 10);
	p1[1] = (void*) arrayOfWords;
	*instrNumPtr = 8;
	*noOfWordsPtr = 3;
	searchString = search_ngram(p1);		//before delete
	CuAssertStrEquals(tc,"hello world !",searchString);
	free(searchString);
	searchString=NULL;
	
	p1[1] = (void*) arrayOfWords;
	*instrNumPtr = 11;
	*noOfWordsPtr = 3;
	searchString = search_ngram(p1);		//after delete
	CuAssertStrEquals(tc,"-1",searchString);
	free(searchString);
	searchString=NULL;
	
	
	for(int i=0;i<noOfWords;i++){
		free(arrayOfWords[i]);												//free 
		arrayOfWords[i] = NULL;
	}
	
	destroyLinearHash(hashTable);
	hashTable = NULL;
	
	free(arrayOfWords);				
	arrayOfWords = NULL;
		
	//free bloomFilter
	freeFilter(topFilter);
	//free array
	destroyTopArray(topArray);
	
	free(p1);
	p1 = NULL;

	printf("End of testing search_ngram\n");
}

void TestSearch_ngram_StaticVersion(CuTest *tc){

	printf("Start of Testing search_ngram_StaticVersion\n");

	HashTable* hashTable = createLinearHash(NOOFBUCKETS, NOOFCELLS);
	BloomFilter *topFilter = initializeFilter(5);		//initialize bloomFilter here
	topKArray *topArray = initializeTopKArray();		//initialize topKArray
	
	//open initForStaticSearch
	FILE *initFile;
	initFile = fopen ("./testingFiles/initForStaticSearch","r");
	int staticDynamic = 1;
	if (initFile!=NULL)
	{
		staticDynamic = initialize(initFile, hashTable);
		fclose (initFile);
	}
	
	if(staticDynamic==0){			//compress
		recreateStructure(hashTable);	
	}
	
	int noOfWords = 3;
	int* noOfWordsPtr = &noOfWords;
	
	char *stringQ1[3] = {"this","is","a"};
	void** p1 = malloc(5*sizeof(void*));
	p1[0] = (void*) hashTable;
	p1[1] = (void*) stringQ1;
	p1[2] = (void*) noOfWordsPtr;
	p1[3] = (void*) topFilter;
	p1[4] = (void*) topArray;
	
	char *result1 = search_ngram_StaticVersion(p1);
	CuAssertStrEquals(tc,"this is a",result1);
	
	char *stringQ2[4] = {"this","is","a","dog"};
	*noOfWordsPtr = 4;
	p1[1] = (void*) stringQ2;
	char *result2 = search_ngram_StaticVersion(p1);
	CuAssertStrEquals(tc,"this is athis is a dog",result2);
	
	char *stringQ3[3] = {"this","is","the"};
	*noOfWordsPtr = 3;
	p1[1] = (void*) stringQ3;
	char *result3 = search_ngram_StaticVersion(p1);
	CuAssertStrEquals(tc,"-1",result3);
	
	destroyLinearHash(hashTable);
	free(topFilter);
	topFilter = NULL;
	destroyTopArray(topArray);
	topArray = NULL;
	
	free(result1);
	result1 = NULL;
	free(result2);
	result2 = NULL;
	free(result3);
	result3 = NULL;
	
	free(p1);
	p1= NULL;
	printf("End of Testing search_ngram_StaticVersion\n");
	
}

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
	
	HashTable* hashTable = createLinearHash(NOOFBUCKETS, NOOFCELLS);
	
	char* myString= malloc(sizeof(char)*(strlen("a")+1));
	strcpy(myString,"a");
	callBasicFuncs(NULL,myString,'D', hashTable,NULL, NULL, 1, 1,NULL);			//will not be deleted because array_of_str does not have any ngrams inside yet
	free(myString);
	
	
	CuAssertPtrNotNull(tc,hashTable);
	CuAssertIntEquals(tc,0,hashTable->buckets[0].position);	
	CuAssertIntEquals(tc,4,hashTable->buckets[0].length);	
	CuAssertPtrEquals(tc,NULL,hashTable->buckets[0].cells[0].dynamicWord);
	CuAssertIntEquals(tc,-1,hashTable->buckets[0].cells[0].noOfChars);
	CuAssertPtrEquals(tc,NULL,hashTable->buckets[0].cells[0].nextWordArray);
	
	
	myString = malloc(sizeof(char)*(strlen("test")+1));
	strcpy(myString,"test");
	callBasicFuncs(NULL,myString,'A', hashTable,NULL, NULL, 1, 2,NULL);			
	free(myString);
	
	myString= malloc(sizeof(char)*(strlen("this is cat")+1));
	strcpy(myString,"this is cat");
	callBasicFuncs(NULL,myString,'A', hashTable,NULL, NULL, 1, 3,NULL);			
	free(myString);
	
	myString= malloc(sizeof(char)*(strlen("this is fox")+1));
	strcpy(myString,"this is fox");
	callBasicFuncs(NULL,myString,'A', hashTable,NULL, NULL, 1, 4,NULL);			
	free(myString);
	
	CuAssertIntEquals(tc,2,hashTable->buckets[0].position);
	CuAssertStrEquals(tc,"test",hashTable->buckets[0].cells[0].word);
	CuAssertStrEquals(tc,"this",hashTable->buckets[0].cells[1].word);
	
	myString= malloc(sizeof(char)*(strlen("test")+1));
	strcpy(myString,"test");
	callBasicFuncs(NULL,myString,'D', hashTable,NULL, NULL, 1, 5,NULL);					//will be deleted
	free(myString);
	
	CuAssertIntEquals(tc,5,hashTable->buckets[0].cells[0].deletionVersion);
	
	myString= malloc(sizeof(char)*(strlen("test")+1));
	strcpy(myString,"test");
	callBasicFuncs(NULL,myString,'A', hashTable,NULL, NULL, 1, 6,NULL);
	free(myString);
	
	myString= malloc(sizeof(char)*(strlen("test is")+1));
	strcpy(myString,"test is");
	callBasicFuncs(NULL,myString,'D', hashTable,NULL, NULL, 1, 7,NULL);			//will not be deleted because full ngram doesn't exist 
	free(myString);


	CuAssertIntEquals(tc,-1,hashTable->buckets[0].cells[1].deletionVersion); 	

	myString= malloc(sizeof(char)*(strlen("this is")+1));
	strcpy(myString,"this is");
	callBasicFuncs(NULL,myString,'D', hashTable,NULL, NULL, 1, 8,NULL);			//will not be deleted because this -> is -> cat and this -> is -> dog exist
	free(myString);
	
	
	CuAssertIntEquals(tc,-1,hashTable->buckets[0].cells[1].deletionVersion); 	
	CuAssertIntEquals(tc,-1,hashTable->buckets[0].cells[1].nextWordArray->array[0].deletionVersion); 
		

	
	myString= malloc(sizeof(char)*(strlen("mouse")+1));
	strcpy(myString,"mouse");
	callBasicFuncs(NULL,myString,'D', hashTable,NULL, NULL, 1, 9,NULL);			//will not be deleted because word does not exist
	free(myString);
	
	
	CuAssertIntEquals(tc,-1,hashTable->buckets[0].cells[1].deletionVersion); 	
	CuAssertIntEquals(tc,-1,hashTable->buckets[0].cells[1].nextWordArray->array[0].deletionVersion); 
	CuAssertIntEquals(tc,-1,hashTable->buckets[0].cells[0].deletionVersion); 	
		
	
	myString= malloc(sizeof(char)*(strlen("heterotransplantations")+1));
	strcpy(myString,"heterotransplantations");
	callBasicFuncs(NULL,myString,'A', hashTable,NULL, NULL, 1, 10,NULL);
	CuAssertStrEquals(tc,"heterotransplantations",hashTable->buckets[0].cells[0].dynamicWord); 
	callBasicFuncs(NULL,myString,'D', hashTable,NULL, NULL, 1, 11,NULL);
	CuAssertIntEquals(tc,11,hashTable->buckets[0].cells[0].deletionVersion); 

	free(myString);
	
	destroyLinearHash(hashTable);
	hashTable = NULL;

}



CuSuite* FuncGetSuite() {		//adding TestFunc Functions into suite
    CuSuite* suite = CuSuiteNew();
    
	SUITE_ADD_TEST(suite, TestInsert_ngram);
	SUITE_ADD_TEST(suite, TestSearch_ngram);
	SUITE_ADD_TEST(suite, TestSearch_ngram_StaticVersion);
    SUITE_ADD_TEST(suite, TestDelete_ngram);
    
    return suite;
}
