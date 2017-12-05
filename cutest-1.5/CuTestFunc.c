#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CuTestFunc.h"

/*
void TestInsert_ngram(CuTest *tc){
	
	arrayOfStructs* array_of_str = malloc(sizeof(arrayOfStructs));
	initializeArray(array_of_str);
	arrayOfStructs* tempArray;
	
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
	strcpy(arrayOfWords[0],"cat");										//check if nextWordArray is created -> insert phrase 'cat dog "big phrase"'
	strcpy(arrayOfWords[1],"dog");
	strcpy(arrayOfWords[2],"whatagreatworldthatisiamflattered");
	insert_ngram(array_of_str, arrayOfWords, noOfWords);
	CuAssertStrEquals(tc,"cat",array_of_str->array[0].word);
	CuAssertPtrEquals(tc,NULL,array_of_str->array[0].dynamicWord);
	CuAssertIntEquals(tc,strlen(array_of_str->array[0].word)+1,array_of_str->array[0].noOfChars);
	CuAssertTrue(tc,!array_of_str->array[0].isDynamic);
	CuAssertTrue(tc,!array_of_str->array[0].isFinal);
	
	
	tempArray = array_of_str->array[0].nextWordArray->array[0].nextWordArray;					//check for big word insertion
	CuAssertStrEquals(tc,"\0",tempArray->array[0].word);
	CuAssertStrEquals(tc,"whatagreatworldthatisiamflattered",tempArray->array[0].dynamicWord);
	CuAssertIntEquals(tc,strlen(tempArray->array[0].dynamicWord)+1,tempArray->array[0].noOfChars);
	CuAssertTrue(tc,tempArray->array[0].isDynamic);
	CuAssertTrue(tc,tempArray->array[0].isFinal);
	
	
	
	tempArray = array_of_str->array[0].nextWordArray;
	CuAssertStrEquals(tc,"dog",tempArray->array[0].word);
	CuAssertTrue(tc,!tempArray->array[0].isFinal);
	
	
	
	strcpy(arrayOfWords[0],"ant");															//check if insert is sorting array -> insert phrase 'ant dog'
	strcpy(arrayOfWords[1],"dog");
	noOfWords = 2;
	insert_ngram(array_of_str, arrayOfWords, noOfWords);
	CuAssertStrEquals(tc,"ant",array_of_str->array[0].word);
	CuAssertStrEquals(tc,"cat",array_of_str->array[1].word);
	CuAssertTrue(tc,!array_of_str->array[0].isFinal);
	CuAssertTrue(tc,!array_of_str->array[1].isFinal);
	tempArray = array_of_str->array[0].nextWordArray;
	CuAssertStrEquals(tc,"dog",tempArray->array[0].word);
	CuAssertTrue(tc,tempArray->array[0].isFinal);
	
	strcpy(arrayOfWords[0],"ant");															//check if insert is sorting nextWordArray -> insert phrase 'ant ant'
	strcpy(arrayOfWords[1],"ant");
	noOfWords = 2;
	insert_ngram(array_of_str, arrayOfWords, noOfWords);
	CuAssertStrEquals(tc,"ant",array_of_str->array[0].word);
	CuAssertStrEquals(tc,"cat",array_of_str->array[1].word);
	CuAssertTrue(tc,!array_of_str->array[0].isFinal);
	CuAssertTrue(tc,!array_of_str->array[1].isFinal);
	tempArray = array_of_str->array[0].nextWordArray;
	CuAssertStrEquals(tc,"ant",tempArray->array[0].word);
	CuAssertStrEquals(tc,"dog",tempArray->array[1].word);
	CuAssertTrue(tc,tempArray->array[0].isFinal);
	
	strcpy(arrayOfWords[0],"ant");															//check insert of a word
	noOfWords = 1;
	insert_ngram(array_of_str, arrayOfWords, noOfWords);
	CuAssertStrEquals(tc,"ant",array_of_str->array[0].word);
	CuAssertTrue(tc,array_of_str->array[0].isFinal);
	
	char* temp = malloc(10*sizeof(char));
	char* number = malloc(3*sizeof(char));
	
	noOfWords = 1;
	for(int i=0;i<12;i++){
		sprintf(number,"%d",i);	
		strcpy(temp,"hello");															//check double of array
		strcat(temp,number);
		strcpy(arrayOfWords[0],temp);										
		insert_ngram(array_of_str, arrayOfWords, noOfWords);
	}
	CuAssertIntEquals(tc,20,array_of_str->length);
	
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
	deleteArray(array_of_str);
	array_of_str = NULL;
}


void TestSearch_ngram(CuTest *tc){
	
	printf("Start of testing search_ngram\n");
	arrayOfStructs* array_of_str = malloc(sizeof(arrayOfStructs));
	initializeArray(array_of_str);
	arrayOfStructs* tempArray;
	
	char** arrayOfWords = malloc(3*sizeof(char*));
	int noOfWords;
	
	arrayOfWords[0] = malloc(10* sizeof(char));
	arrayOfWords[1] = malloc(40* sizeof(char));
	arrayOfWords[2] = malloc(10* sizeof(char));
	
	
	noOfWords = 3;
	strcpy(arrayOfWords[0],"cat");										//get search phrase ->found
	strcpy(arrayOfWords[1],"dogisthebestpetexceptforcats");
	strcpy(arrayOfWords[2],"record");

	insert_ngram(array_of_str, arrayOfWords, noOfWords);
	
	char* searchString;
	searchString = search_ngram(array_of_str, arrayOfWords, noOfWords);
	
	CuAssertStrEquals(tc,"cat dogisthebestpetexceptforcats record",searchString);
	free(searchString);
	searchString=NULL;
	
	for(int i=0;i<noOfWords;i++){												//free 
		free(arrayOfWords[i]);
		arrayOfWords[i] = NULL;
	}
	
	noOfWords = 1;
	arrayOfWords[0] = malloc(10* sizeof(char));														//not found
	strcpy(arrayOfWords[0],"init");
	searchString = search_ngram(array_of_str, arrayOfWords, noOfWords);
	CuAssertStrEquals(tc,"-1",searchString);
	free(searchString);
	searchString=NULL;
	
												//free 
	free(arrayOfWords[0]);
	arrayOfWords[0] = NULL;

	
	
	arrayOfWords[0] = malloc(10* sizeof(char));

	noOfWords = 1;
	strcpy(arrayOfWords[0],"cat");										//get search phrase ->found
	insert_ngram(array_of_str, arrayOfWords, 1);
	free(arrayOfWords[0]);
	arrayOfWords[0] = NULL;
	
	arrayOfWords[0] = malloc(40* sizeof(char));
	strcpy(arrayOfWords[0],"dogisthebestpetexceptforcats");
	insert_ngram(array_of_str, arrayOfWords, 1);
	free(arrayOfWords[0]);
	arrayOfWords[0] = NULL;
	
	arrayOfWords[0] = malloc(10* sizeof(char));
	strcpy(arrayOfWords[0],"record");
	insert_ngram(array_of_str, arrayOfWords, 1);
	free(arrayOfWords[0]);
	arrayOfWords[0] = NULL;
	
	char** temp = malloc(3*sizeof(char*));
	temp[0] =  malloc(10* sizeof(char));
	temp[1] =  malloc(10* sizeof(char));
	temp[2] =  malloc(40* sizeof(char));
	strcpy(temp[0],"cat");
	strcpy(temp[1],"is");
	strcpy(temp[2],"dogisthebestpetexceptforcats");
	
	searchString = search_ngram(array_of_str, temp, 3);
	
	//found multiple ones
	CuAssertStrEquals(tc,"catdogisthebestpetexceptforcats",searchString);
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
	deleteArray(array_of_str);
	array_of_str = NULL;
	
	printf("End of testing search_ngram\n");
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
	/*
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
	CuAssertPtrEquals(tc,NULL,array_of_str->array[array_of_str->position].dynamicWord);
	CuAssertIntEquals(tc,0,strlen(array_of_str->array[array_of_str->position].word));
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
		
	
	myString= malloc(sizeof(char)*(strlen("whataworldandanimecommunity")+1));
	strcpy(myString,"whataworldandanimecommunity");
	callBasicFuncs(myString,array_of_str,'A');
	CuAssertStrEquals(tc,"whataworldandanimecommunity",array_of_str->array[2].dynamicWord); 
	callBasicFuncs(myString,array_of_str,'D');
	CuAssertPtrEquals(tc,NULL,array_of_str->array[2].dynamicWord); 	
	free(myString);
	
	deleteArray(array_of_str);
	array_of_str=NULL;
	CuAssertPtrEquals(tc,NULL,array_of_str);

}



CuSuite* FuncGetSuite() {		//adding TestFunc Functions into suite
    CuSuite* suite = CuSuiteNew();
    
	SUITE_ADD_TEST(suite, TestInsert_ngram);
	SUITE_ADD_TEST(suite, TestSearch_ngram);
    SUITE_ADD_TEST(suite, TestDelete_ngram);

    
    return suite;
}*/
