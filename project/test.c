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
	test_stringToArray();
	test_initialize();			
	test_callBasicFuncs();		
	test_binarySearch();		
	test_insertionSort();
	test_deletionSort();		
	test_executeQueryFile();	
	test_checkIfStringExists();	
	
	test_initializeArray();
	test_doubleLength();
	test_deleteArray();				
	test_deleteDataNode();
	
	test_insert_ngram();			
	test_search_ngram();			
	test_delete_ngram();			
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
  	deleteStack(myStack);			//delete when stack is not null and is initialized
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
	free(array);
	array = NULL;
	assert(array==NULL);
	
	length=1;										//delete array with one element
	char** array2 = malloc(sizeof(char*)*length);
	array2[0]= malloc(sizeof(char)*(strlen("Hello")+1));
	strcpy(array2[0],"Hello");
	deleteArrayOfWords(array2, length);
	free(array2);
	array2 = NULL;
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
	free(array3);
	array3 = NULL;
	assert(array3==NULL);

}

void test_stringToArray(){
	char* ngram=NULL;
	arrayWords* array = stringToArray(ngram);
	assert(array!=NULL && array->length==0 && array->words!=NULL);
	deleteArrayOfWords(array->words, array->length);
	free(array->words);
	array->words = NULL;
	free(array);
	array=NULL;
	
	ngram=malloc((strlen("The fox is bad and the cat is good.")+1) * sizeof(char));
	strcpy(ngram,"The fox is bad and the cat is good.");
	array = stringToArray(ngram);
	assert(array!=NULL && array->length==9 && array->words!=NULL);
	
	assert(strcmp(array->words[0],"The")==0);
	assert(strcmp(array->words[1],"fox")==0);
	assert(strcmp(array->words[2],"is")==0);
	assert(strcmp(array->words[3],"bad")==0);
	assert(strcmp(array->words[4],"and")==0);
	assert(strcmp(array->words[5],"the")==0);
	assert(strcmp(array->words[6],"cat")==0);
	assert(strcmp(array->words[7],"is")==0);
	assert(strcmp(array->words[8],"good.")==0);
	
	deleteArrayOfWords(array->words, array->length);
	free(array->words);
	array->words = NULL;
	free(array);
	array=NULL;
	
	free(ngram);
	ngram=NULL;
}

void test_initialize(){
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
	assert(structureTree->length==10 && structureTree->position==3 && structureTree->array!=NULL);
	
	assert(strcmp(structureTree->array[0].word,"hi")==0 && structureTree->array[0].nextWordArray==NULL && structureTree->array[0].isFinal==true);
	assert(strcmp(structureTree->array[1].word,"test")==0 && structureTree->array[1].nextWordArray==NULL && structureTree->array[1].isFinal==true);
	assert(strcmp(structureTree->array[2].word,"this")==0 && structureTree->array[2].nextWordArray!=NULL && structureTree->array[2].isFinal==false);
	
	arrayOfStructs* nextArray = structureTree->array[2].nextWordArray; 
	assert(nextArray->length==10 && nextArray->position==2 && nextArray->array!=NULL);
	
	assert(strcmp(nextArray->array[0].word,"cat")==0 && nextArray->array[0].nextWordArray==NULL && nextArray->array[0].isFinal==true);
	assert(strcmp(nextArray->array[1].word,"dog")==0 && nextArray->array[1].nextWordArray==NULL && nextArray->array[1].isFinal==true);
	assert(nextArray->position==2);
	
	deleteArray(structureTree);
	structureTree=NULL;
	assert(structureTree==NULL);
}


void test_binarySearch(){

	arrayOfStructs* array_of_str = malloc(sizeof(arrayOfStructs));
	initializeArray(array_of_str);
	checkItemExists* getPosition;
	
	dataNode *item = malloc(sizeof(dataNode));
	item->word = malloc(10 * sizeof(char));
	getPosition = binarySearch(array_of_str,item,0,array_of_str->position,NULL);			//array is empty
	assert(getPosition->position==-1 && getPosition->exists==false);
	
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
	assert(getPosition->position==0 && getPosition->exists==true);
		
	free(getPosition);
	getPosition = NULL;	
		
	strcpy(item->word,"hello");																//non existing word
	getPosition = binarySearch(array_of_str,item,0,array_of_str->position,NULL);
	assert(getPosition->position==-1 && getPosition->exists==false);				
	
	free(getPosition);
	getPosition = NULL;
	
	deleteDataNode(item);
	free(item);
	item = NULL;
	deleteArray(array_of_str);
	array_of_str = NULL;
	
}

void test_insertionSort(){

	arrayOfStructs* array_of_str = malloc(sizeof(arrayOfStructs));
	initializeArray(array_of_str);

	dataNode *item = malloc(sizeof(dataNode));
	item->word=NULL;
	checkItemExists* getPosition = insertionSort(array_of_str,item,array_of_str->position);   //array is empty  
	assert(getPosition->position==0 && getPosition->exists==false);
	
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
	assert(getPosition->position!=-1 && getPosition->exists==true);
	
	free(getPosition);
	getPosition = NULL;
	
	
	dataNode *item3 = malloc(sizeof(dataNode));
	item3->word = malloc((strlen("hello")+1) * sizeof(char));
	strcpy(item3->word,"hello");     
	item3->nextWordArray=NULL;
	
	getPosition = insertionSort(array_of_str,item3,array_of_str->position);
	array_of_str->position++;               //non existing word
	assert(getPosition->position==2 && getPosition->exists==false);      
	
	free(getPosition);
	getPosition = NULL;
	
	
	dataNode *item4 = malloc(sizeof(dataNode));
	item4->word = malloc((strlen("ant")+1) * sizeof(char));
	strcpy(item4->word,"ant");     
	item4->nextWordArray=NULL;
	
	getPosition = insertionSort(array_of_str,item4,array_of_str->position);
	array_of_str->position++;               	//non existing word

	assert(getPosition->position==0 && getPosition->exists==false); 
	assert(strcmp(array_of_str->array[0].word,"ant")==0);
	assert(strcmp(array_of_str->array[1].word,"cat")==0);
	assert(strcmp(array_of_str->array[2].word,"dog")==0);
	assert(strcmp(array_of_str->array[3].word,"hello")==0);    
	
	free(getPosition);
	getPosition = NULL;
	
	
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
	
	free(array_of_str->array);
	array_of_str->array = NULL;
	free(array_of_str);
	array_of_str = NULL;
	

}

void test_deletionSort(){

	arrayOfStructs* array_of_str = malloc(sizeof(arrayOfStructs));
	initializeArray(array_of_str);
	
	char** item = malloc(1*sizeof(char*));
	item[0] = malloc((strlen("hello")+1) * sizeof(char));
	strcpy(item[0],"hello");																
	insert_ngram(array_of_str, item, 1);							//add 'hello'
	

	deletionSort(array_of_str,0,array_of_str->position);		//delete only element
	assert(array_of_str!=NULL);
	assert(array_of_str->array!=NULL);
	assert(array_of_str->length==10);
	assert(array_of_str->position==0);
	
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
	
	assert(array_of_str!=NULL);
	assert(array_of_str->array!=NULL);
	assert(array_of_str->length==10);
	assert(array_of_str->position==3);
	
	assert(strcmp(array_of_str->array[0].word,"hello")==0);
	assert(strcmp(array_of_str->array[1].word,"test")==0);
	assert(strcmp(array_of_str->array[2].word,"the")==0);
	
	item = malloc(1*sizeof(char*));
	item[0] = malloc((strlen("this")+1) * sizeof(char));
	strcpy(item[0],"this");																
	insert_ngram(array_of_str, item, 1);							//add 'this'
	
	free(item[0]);
	item[0]=NULL;
	free(item);
	item=NULL;
	
	deletionSort(array_of_str,1,array_of_str->position);		//from hello, test, the, this delete test
	assert(array_of_str!=NULL);
	assert(array_of_str->array!=NULL);
	assert(array_of_str->length==10);
	assert(array_of_str->position==3);
	assert(strcmp(array_of_str->array[0].word,"hello")==0);	
	assert(strcmp(array_of_str->array[1].word,"the")==0);
	assert(strcmp(array_of_str->array[2].word,"this")==0);
	
	
	deleteArray(array_of_str);
	array_of_str=NULL;
}


void test_executeQueryFile(){
	
	printf("Start of Testing executeQueryFile\n");
	FILE * queryFile;
	queryFile = fopen ("testingFiles/correct1batch","r");
	arrayOfStructs* array_of_str = malloc(sizeof(arrayOfStructs));
	initializeArray(array_of_str);
	assert(executeQueryFile(queryFile,array_of_str)==1);					//correct file
	fclose (queryFile);
	array_of_str = NULL;
	assert(array_of_str==NULL);
	
	queryFile = fopen ("testingFiles/correctMultipleBatches","r");
	array_of_str = malloc(sizeof(arrayOfStructs));
	initializeArray(array_of_str);
	assert(executeQueryFile(queryFile,array_of_str)==1);					//correct file with multiple batches
	fclose (queryFile);
	array_of_str = NULL;
	assert(array_of_str==NULL);
	
	queryFile = fopen ("testingFiles/incorrectNoF","r");
	array_of_str = malloc(sizeof(arrayOfStructs));
	initializeArray(array_of_str);
	assert(executeQueryFile(queryFile,array_of_str)==0);					//file does not end with F
	fclose (queryFile);
	array_of_str = NULL;
	assert(array_of_str==NULL);
	
	queryFile = fopen ("testingFiles/IncorrectLetter","r");
	array_of_str = malloc(sizeof(arrayOfStructs));
	initializeArray(array_of_str);
	assert(executeQueryFile(queryFile,array_of_str)==0);					//query of file starts with sth else other than A,D,Q,F
	fclose (queryFile);
	array_of_str = NULL;
	assert(array_of_str==NULL);
	
	printf("End of Testing executeQueryFile\n");

}

void test_checkIfStringExists(){

	int noOfWords=0;
	char** array=NULL;
	char* str = malloc((strlen("")+1)* sizeof(char));
	assert(checkIfStringExists(array, noOfWords, str)==0);			//checking null array
	
	noOfWords=0;
	array=malloc(noOfWords * sizeof(char*));
	str = realloc(str,(strlen("")+1)* sizeof(char));
	strcpy(str,"");
	assert(checkIfStringExists(array, noOfWords, str)==0);			//checking array with no elements
	
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
	assert(checkIfStringExists(array, noOfWords, str)==0);			//checking array with word that does not exist inside it
	
	str = realloc(str,(strlen("dog")+1)* sizeof(char));
	strcpy(str,"dog");
	assert(checkIfStringExists(array, noOfWords, str)==1);			//checking array with word that does exist inside it
	
	free(str);
	str=NULL;
	
	for(int i=0; i<noOfWords; i++){
		free(array[i]);
		array[i]=NULL;
	}
	free(array);
	array=NULL;

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
	insert_ngram(array_of_str, arrayOfWords, noOfWords);
	assert(strcmp(array_of_str->array[0].word,"cat")==0 && array_of_str->array[0].isFinal == false);
	tempArray = array_of_str->array[0].nextWordArray;
	assert(strcmp(tempArray->array[0].word,"dog")==0 && tempArray->array[0].isFinal == false);
	tempArray = tempArray->array[0].nextWordArray;
	assert(strcmp(tempArray->array[0].word,"record")==0 && tempArray->array[0].isFinal == true);
	
	
	
	strcpy(arrayOfWords[0],"ant");															//check if insert is sorting array -> insert phrase 'ant dog'
	strcpy(arrayOfWords[1],"dog");
	noOfWords = 2;
	insert_ngram(array_of_str, arrayOfWords, noOfWords);
	assert(strcmp(array_of_str->array[0].word,"ant")==0 && strcmp(array_of_str->array[1].word,"cat")==0);
	assert(array_of_str->array[0].isFinal == false && array_of_str->array[1].isFinal == false);
	tempArray = array_of_str->array[0].nextWordArray;
	assert(strcmp(tempArray->array[0].word,"dog")==0 && tempArray->array[0].isFinal==true);
	
	strcpy(arrayOfWords[0],"ant");															//check if insert is sorting nextWordArray -> insert phrase 'ant ant'
	strcpy(arrayOfWords[1],"ant");
	noOfWords = 2;
	insert_ngram(array_of_str, arrayOfWords, noOfWords);
	assert(strcmp(array_of_str->array[0].word,"ant")==0 && strcmp(array_of_str->array[1].word,"cat")==0);
	assert(array_of_str->array[0].isFinal == false && array_of_str->array[1].isFinal == false);
	tempArray = array_of_str->array[0].nextWordArray;
	assert(strcmp(tempArray->array[0].word,"ant")==0 && strcmp(tempArray->array[1].word,"dog")==0);
	assert(tempArray->array[0].isFinal==true);
	
	strcpy(arrayOfWords[0],"ant");															//check insert of a word
	noOfWords = 1;
	insert_ngram(array_of_str, arrayOfWords, noOfWords);
	assert(strcmp(array_of_str->array[0].word,"ant")==0 && array_of_str->array[0].isFinal == true);
	
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


void test_search_ngram(){
	
	printf("Start of testing search_ngram\n");
	arrayOfStructs* array_of_str = malloc(sizeof(arrayOfStructs));
	initializeArray(array_of_str);
	arrayOfStructs* tempArray;
	
	char** arrayOfWords = malloc(3*sizeof(char*));
	int noOfWords;
	
	arrayOfWords[0] = malloc(10* sizeof(char));
	arrayOfWords[1] = malloc(10* sizeof(char));
	arrayOfWords[2] = malloc(10* sizeof(char));
	
	
	noOfWords = 3;
	strcpy(arrayOfWords[0],"cat");										//get search phrase
	strcpy(arrayOfWords[1],"dog");
	strcpy(arrayOfWords[2],"record");

	insert_ngram(array_of_str, arrayOfWords, noOfWords);
	
	char* searchString = search_ngram(array_of_str, arrayOfWords, noOfWords);
	
	
	for(int i=0;i<3;i++){												//free 
		free(arrayOfWords[i]);
		arrayOfWords[i] = NULL;
	}
	
	free(searchString);
	searchString=NULL;
	
	free(arrayOfWords);
	arrayOfWords = NULL;
	deleteArray(array_of_str);
	array_of_str = NULL;
	
	printf("End of testing search_ngram\n");
}

void test_delete_ngram(){

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
	
	assert(array_of_str!=NULL && array_of_str->position==0 && array_of_str->length==10 && array_of_str->array!=NULL && array_of_str->array[array_of_str->position].word==NULL);
	assert(array_of_str->array[array_of_str->position].nextWordArray==NULL);
	
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
	
	assert(array_of_str->position==2);
	
	myString= malloc(sizeof(char)*(strlen("test it")+1));
	strcpy(myString,"test it");
	callBasicFuncs(myString,array_of_str,'D');		//will be deleted
	free(myString);
	
	assert(array_of_str->position==1);
	assert(strcmp(array_of_str->array[0].word,"this")==0);
	assert(strcmp(array_of_str->array[0].nextWordArray->array[0].word,"is")==0);	
	assert(array_of_str->array[0].nextWordArray->position==1);	
	assert(strcmp(array_of_str->array[0].nextWordArray->array[0].nextWordArray->array[0].word,"cat")==0);
	assert(strcmp(array_of_str->array[0].nextWordArray->array[0].nextWordArray->array[1].word,"fox")==0);	
	assert(array_of_str->array[0].nextWordArray->array[0].nextWordArray->position==2);	
	
	
	
	myString= malloc(sizeof(char)*(strlen("test")+1));
	strcpy(myString,"test");
	callBasicFuncs(myString,array_of_str,'A');
	free(myString);
	
	myString= malloc(sizeof(char)*(strlen("test is")+1));
	strcpy(myString,"test is");
	callBasicFuncs(myString,array_of_str,'D');			//will not be deleted because full ngram doesn't exist 
	free(myString);
	
	assert(strcmp(array_of_str->array[0].word,"test")==0);
	assert(strcmp(array_of_str->array[1].word,"this")==0);
	assert(array_of_str->position==2);	
	assert(strcmp(array_of_str->array[1].nextWordArray->array[0].word,"is")==0);
	assert(array_of_str->array[1].nextWordArray->position==1);		
	assert(strcmp(array_of_str->array[1].nextWordArray->array[0].nextWordArray->array[0].word,"cat")==0);
	assert(strcmp(array_of_str->array[1].nextWordArray->array[0].nextWordArray->array[1].word,"fox")==0);
	assert(array_of_str->array[1].nextWordArray->array[0].nextWordArray->position==2);			
	
	
	myString= malloc(sizeof(char)*(strlen("this is")+1));
	strcpy(myString,"this is");
	callBasicFuncs(myString,array_of_str,'D');			//will not be deleted because this -> is -> cat and this -> is -> dog exist
	free(myString);
	
	assert(strcmp(array_of_str->array[0].word,"test")==0);
	assert(strcmp(array_of_str->array[1].word,"this")==0);
	assert(array_of_str->position==2);	
	assert(strcmp(array_of_str->array[1].nextWordArray->array[0].word,"is")==0);
	assert(array_of_str->array[1].nextWordArray->position==1);		
	assert(strcmp(array_of_str->array[1].nextWordArray->array[0].nextWordArray->array[0].word,"cat")==0);
	assert(strcmp(array_of_str->array[1].nextWordArray->array[0].nextWordArray->array[1].word,"fox")==0);
	assert(array_of_str->array[1].nextWordArray->array[0].nextWordArray->position==2);	

	
	myString= malloc(sizeof(char)*(strlen("mouse")+1));
	strcpy(myString,"mouse");
	callBasicFuncs(myString,array_of_str,'D');			//will not be deleted because word does not exist
	free(myString);
	
	assert(strcmp(array_of_str->array[0].word,"test")==0);
	assert(strcmp(array_of_str->array[1].word,"this")==0);
	assert(array_of_str->position==2);	
	assert(strcmp(array_of_str->array[1].nextWordArray->array[0].word,"is")==0);
	assert(array_of_str->array[1].nextWordArray->position==1);		
	assert(strcmp(array_of_str->array[1].nextWordArray->array[0].nextWordArray->array[0].word,"cat")==0);
	assert(strcmp(array_of_str->array[1].nextWordArray->array[0].nextWordArray->array[1].word,"fox")==0);
	assert(array_of_str->array[1].nextWordArray->array[0].nextWordArray->position==2);		
	
	
	
	deleteArray(array_of_str);
	array_of_str=NULL;
	assert(array_of_str==NULL);

	
}

void test_callBasicFuncs(){
 
	arrayOfStructs* array_of_str = malloc(sizeof(arrayOfStructs));
	initializeArray(array_of_str);
	char* ngram = malloc(20*sizeof(char));
	strcpy(ngram,"A cat in my shoe");

	callBasicFuncs(ngram, array_of_str, 'A');
	callBasicFuncs(ngram, array_of_str, 'Q');
	callBasicFuncs(ngram, array_of_str, 'D');

	free(ngram);
	ngram = NULL;
	deleteArray(array_of_str);
	array_of_str = NULL;
}

















