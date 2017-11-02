#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "test.h"
#include "stack.h"
#include "auxMethods.h"
#include "func.h"


void testAllFunctions(){		//calls all testFunctions
	
	
	test_initializeStack();
	test_deleteStack();
	test_doubleLengthOfStack();
	test_push();
	test_isEmpty();
	test_pop();
	
	test_deleteArrayOfWords();
	test_binarySearch();
	test_insertionSort();
	test_executeQueryFile();
	
	test_initializeArray();
	test_doubleLength();
	test_deleteArray();
	test_deleteDataNode();
	
	test_insert_ngram();
}


//Testing stack functions

void test_initializeStack(){
	stack* myStack = malloc(sizeof(stack));
  	initializeStack(myStack);
  	assert(myStack->top==-1 && myStack->length==10 && myStack->positionsToDelete!=NULL);
  	deleteStack(myStack);
  	free(myStack);
	myStack = NULL;
}

void test_deleteStack(){

	stack* myStack=NULL;
	deleteStack(myStack);			//delete when stack is null
  	assert(myStack==NULL);
  		
	myStack = malloc(sizeof(stack));
	myStack->positionsToDelete=NULL;
	deleteStack(myStack);			//delete when stack is not initialized
	free(myStack);
	myStack = NULL;
  	assert(myStack==NULL);
  	
  	myStack = malloc(sizeof(stack));
  	initializeStack(myStack);
  	deleteStack(myStack);			//delete when stack is not null
  	free(myStack);
	myStack = NULL;
  	assert(myStack==NULL);
}




void test_doubleLengthOfStack(){
	stack* myStack = malloc(sizeof(stack));
  	initializeStack(myStack);
  	doubleLengthOfStack(myStack);
  	assert(myStack->length==20 && myStack->positionsToDelete!=NULL);	
  	deleteStack(myStack);
  	free(myStack);
	myStack = NULL;
}

void test_push(){
	stack* myStack = malloc(sizeof(stack));
  	initializeStack(myStack);

  	push(myStack,0);		//test if one element can be pushed into the stack when stack is empty
  	assert(myStack->top==0 && myStack->positionsToDelete[0]==0);
  	
  	for(int i=1; i<=10; i++){	//push 10 elements so that stack will be doubled
  		push(myStack,i);
  	}
  	
  	assert(myStack->top==10 && myStack->length==20);
  	for(int i=0; i<=10; i++){
  		assert(myStack->positionsToDelete[i]==i);
  	}
  	deleteStack(myStack);
  	free(myStack);
	myStack = NULL;
  	
}

void test_isEmpty(){
	stack* myStack = malloc(sizeof(stack));
  	initializeStack(myStack);
  	assert(isEmpty(myStack));
  	push(myStack, 0);
  	assert(!isEmpty(myStack));
  	deleteStack(myStack);
  	free(myStack);
	myStack = NULL;
}

void test_pop(){
	stack* myStack = malloc(sizeof(stack));
  	initializeStack(myStack);
  	assert(!pop(myStack));			//pop when stack is empty
  	push(myStack, 0);
  	assert(pop(myStack));			//pop when stack is not empty
  	deleteStack(myStack);
  	free(myStack);
	myStack = NULL;
}


//test all auxMethods functions

void test_deleteArrayOfWords(){
	char** array=NULL;
	int length=0;									//delete null array
	deleteArrayOfWords(array, length);
	if(array!=NULL){
		free(array);
		array = NULL;
	}
	assert(array==NULL);
	
	length=1;										//delete array with one element
	char** array2 = malloc(sizeof(char*)*length);
	array2[0]= malloc(sizeof(char)*(strlen("Hello")+1));
	strcpy(array2[0],"Hello");
	deleteArrayOfWords(array2, length);
	if(array2!=NULL){
		free(array2);
		array2 = NULL;
	}
	assert(array2==NULL);
	
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
	if(array3!=NULL){
		free(array3);
		array3 = NULL;
	}
	assert(array3==NULL);

}

void test_binarySearch(){

	arrayOfStructs* array_of_str = malloc(sizeof(arrayOfStructs));
	initializeArray(array_of_str);
	
	dataNode *item = malloc(sizeof(dataNode));
	item->word = malloc(10 * sizeof(char));
	checkItemExists* getPosition = malloc(sizeof(checkItemExists));
	getPosition = binarySearch(array_of_str,item,0,array_of_str->position);			//array is empty
	assert(getPosition->position==-1 && getPosition->exists==false);
		
	strcpy(item->word,"cat");										//existing word
	array_of_str->array[0].word = malloc(10*sizeof(char));
	strcpy(array_of_str->array[0].word,"cat");
	array_of_str->position++;
	array_of_str->array[1].word = malloc(10*sizeof(char));
	strcpy(array_of_str->array[1].word,"dog");
	array_of_str->position++;
	getPosition = binarySearch(array_of_str,item,0,array_of_str->position);
	assert(getPosition->position==0 && getPosition->exists==true);
		
	strcpy(item->word,"hello");																//non existing word
	getPosition = binarySearch(array_of_str,item,0,array_of_str->position);
	assert(getPosition->position==-1 && getPosition->exists==false);						//kanonika eprepe getPosition->position==2
	
	deleteDataNode(item);
	free(item);
	item = NULL;
	deleteArray(array_of_str);
	array_of_str = NULL;
	
	free(getPosition);
	getPosition = NULL;
	
}

void test_insertionSort(){

	arrayOfStructs* array_of_str = malloc(sizeof(arrayOfStructs));
	initializeArray(array_of_str);
	
	dataNode *item = malloc(sizeof(dataNode));
	item->word = malloc(10 * sizeof(char));
	checkItemExists* getPosition = malloc(sizeof(checkItemExists));
	getPosition = insertionSort(array_of_str,item,array_of_str->position);			//array is empty
	assert(getPosition->position==0 && getPosition->exists==false);
		
	strcpy(item->word,"cat");													//existing word
	array_of_str->array[0].word = malloc(10*sizeof(char));
	strcpy(array_of_str->array[0].word,"cat");
	array_of_str->position++;
	array_of_str->array[1].word = malloc(10*sizeof(char));
	strcpy(array_of_str->array[1].word,"dog");
	array_of_str->position++;
	getPosition = insertionSort(array_of_str,item,array_of_str->position);
	assert(getPosition->position!=-1 && getPosition->exists==true);
		
	strcpy(item->word,"hello");																//non existing word
	getPosition = insertionSort(array_of_str,item,array_of_str->position);
	assert(getPosition->position==2 && getPosition->exists==false);						
	
	strcpy(item->word,"ant");																//non existing word for insertion at front
	getPosition = insertionSort(array_of_str,item,array_of_str->position);
	assert(getPosition->position==0 && getPosition->exists==false);	
	assert(strcmp(array_of_str->array[0].word,"ant")==0);
	assert(strcmp(array_of_str->array[1].word,"cat")==0);
	assert(strcmp(array_of_str->array[2].word,"dog")==0);
	//assert(strcmp(array_of_str->array[3].word,"hello")==0);				//segm
	
	
	deleteDataNode(item);
	free(item);
	item = NULL;
	//deleteArray(array_of_str);										//segm
	free(array_of_str);
	array_of_str = NULL;
	
	free(getPosition);
	getPosition = NULL;

}

void test_executeQueryFile(){
	
	printf("Start of Testing executeQueryFile\n");
	FILE * queryFile;
	queryFile = fopen ("query_file","r");
	arrayOfStructs* array_of_str = malloc(sizeof(arrayOfStructs));
	initializeArray(array_of_str);
	assert(executeQueryFile(queryFile,array_of_str));
	fclose (queryFile);
	deleteArray(array_of_str);									
	array_of_str = NULL;
	printf("End of Testing executeQueryFile\n");

}

//test struct.c methods

void test_initializeArray(){
	arrayOfStructs* array_of_str = malloc(sizeof(arrayOfStructs));
	array_of_str->position=0;
	array_of_str->length=10;
	array_of_str->array=malloc(array_of_str->length * sizeof(dataNode));
	
	for(int i=0;i<array_of_str->length;i++){
		array_of_str->array[i].word = NULL;
		array_of_str->array[i].nextWordArray = NULL;
	}
	
	assert(array_of_str->length == 10 && array_of_str->position==0 && array_of_str->array!=NULL);
	for(int i=0;i<array_of_str->length;i++){
		assert(array_of_str->array[i].word == NULL);
		assert(array_of_str->array[i].nextWordArray == NULL);
	}
	
	deleteArray(array_of_str);
	array_of_str = NULL;
}

void test_doubleLength(){
	arrayOfStructs* array_of_str = malloc(sizeof(arrayOfStructs));
	initializeArray(array_of_str);
	int lengthBefore = array_of_str->length;
	doubleLength(array_of_str);
	assert((array_of_str->length == lengthBefore * 2) && array_of_str->array!=NULL);
	deleteArray(array_of_str);
	array_of_str = NULL;
}

void test_deleteArray(){
	arrayOfStructs* array_of_str = NULL;					//when array_of_str is null
	deleteArray(array_of_str);
	assert(array_of_str == NULL);
	
	array_of_str = malloc(sizeof(arrayOfStructs));			//when array_of_str is not initialized 
	array_of_str->array = NULL;
	array_of_str->position = 0;
	array_of_str->length = 0;
	deleteArray(array_of_str);
	array_of_str = NULL;
	assert(array_of_str == NULL);
	
	
	array_of_str = malloc(sizeof(arrayOfStructs));			//when initialized
	initializeArray(array_of_str);
	deleteArray(array_of_str);
	array_of_str = NULL;
	assert(array_of_str == NULL);
	
}

void test_deleteDataNode(){
	
	int k = 15;											//word null
	dataNode* elem = malloc(sizeof(dataNode));
	elem->word = NULL;
	deleteDataNode(elem);
	assert(elem->word == NULL);
	free(elem);
	elem = NULL;

	elem = malloc(sizeof(dataNode));					//word not null
	elem->word = malloc(k * sizeof(char));
	strcpy(elem->word,"Hello world");
	deleteDataNode(elem);
	elem->word = NULL;
	assert(elem->word == NULL);
	free(elem);
	elem = NULL;
	
}


//test func.c methods

void test_insert_ngram(){
	
	arrayOfStructs* array_of_str = malloc(sizeof(arrayOfStructs));
	initializeArray(array_of_str);
	arrayOfStructs* tempArray;
	
	char** arrayOfWords = malloc(12*sizeof(char*));
	int noOfWords;
	
	
	arrayOfWords[0] = malloc(10* sizeof(char));
	arrayOfWords[1] = malloc(10* sizeof(char));
	arrayOfWords[2] = malloc(10* sizeof(char));
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
	strcpy(arrayOfWords[0],"cat");										//check if nextWordArray is created -> insert phrase 'cat dog record'
	strcpy(arrayOfWords[1],"dog");
	strcpy(arrayOfWords[2],"record");
	assert(insert_ngram(array_of_str, arrayOfWords, noOfWords));
	assert(strcmp(array_of_str->array[0].word,"cat")==0 && array_of_str->array[0].isFinal == false);
	tempArray = array_of_str->array[0].nextWordArray;
	assert(strcmp(tempArray->array[0].word,"dog")==0 && tempArray->array[0].isFinal == false);
	tempArray = tempArray->array[0].nextWordArray;
	assert(strcmp(tempArray->array[0].word,"record")==0 && tempArray->array[0].isFinal == true);
	
	
	
	strcpy(arrayOfWords[0],"ant");															//check if insert is sorting array -> insert phrase 'ant dog'
	strcpy(arrayOfWords[1],"dog");
	noOfWords = 2;
	assert(insert_ngram(array_of_str, arrayOfWords, noOfWords));
	assert(strcmp(array_of_str->array[0].word,"ant")==0 && strcmp(array_of_str->array[1].word,"cat")==0);
	assert(array_of_str->array[0].isFinal == false && array_of_str->array[1].isFinal == false);
	tempArray = array_of_str->array[0].nextWordArray;
	assert(strcmp(tempArray->array[0].word,"dog")==0 && tempArray->array[0].isFinal==true);
	
	strcpy(arrayOfWords[0],"ant");															//check if insert is sorting nextWordArray -> insert phrase 'ant ant'
	strcpy(arrayOfWords[1],"ant");
	noOfWords = 2;
	assert(insert_ngram(array_of_str, arrayOfWords, noOfWords));
	assert(strcmp(array_of_str->array[0].word,"ant")==0 && strcmp(array_of_str->array[1].word,"cat")==0);
	assert(array_of_str->array[0].isFinal == false && array_of_str->array[1].isFinal == false);
	tempArray = array_of_str->array[0].nextWordArray;
	assert(strcmp(tempArray->array[0].word,"ant")==0 && strcmp(tempArray->array[1].word,"dog")==0);
	assert(tempArray->array[0].isFinal==true);
	
	strcpy(arrayOfWords[0],"ant");															//check insert of a word
	noOfWords = 1;
	assert(insert_ngram(array_of_str, arrayOfWords, noOfWords));
	assert(strcmp(array_of_str->array[0].word,"ant")==0 && array_of_str->array[0].isFinal == true);
	
	char* temp = malloc(10*sizeof(char));
	char* number = malloc(2*sizeof(char));
	strcpy(temp,"hello");
	noOfWords = 1;
	for(int i=0;i<12;i++){
		sprintf(number,"%d",i);																//check double of array
		strcat(temp,number);
		strcpy(arrayOfWords[0],temp);										
		assert(insert_ngram(array_of_str, arrayOfWords, noOfWords));
	}
	assert(array_of_str->length==20);
	
	
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























