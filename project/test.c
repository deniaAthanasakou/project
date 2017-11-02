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
	//test_initialize();			//MEMORY LEAK BECAUSE OF BINARY
	
	//test_binarySearch();
	//test_insertionSort();
	test_deletionSort();			//PROVLIMA sto deleteArray sega
	//test_executeQueryFile();
	
	/*test_initializeArray();
	test_doubleLength();
	test_deleteArray();
	test_deleteDataNode();
	
	test_insert_ngram();
	test_search_ngram();*/
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
}

void test_initialize(){
	/*text inside test_initialize:
		test
		hi
		this dog
		this cat
	*/

	arrayOfStructs* structureTree = (arrayOfStructs*) malloc(1 * sizeof(arrayOfStructs));
	FILE * initFile;
	initFile = fopen ("test_initialize","r");
	if (initFile!=NULL)
	{
		assert(initialize(initFile, structureTree));
		fclose (initFile);
	}
	assert(structureTree->length==10 && structureTree->position==3 && structureTree->array!=NULL);
	
	assert(strcmp(structureTree->array[0].word,"hi")==0 && structureTree->array[0].nextWordArray!=NULL && structureTree->array[0].isFinal==true);
	assert(strcmp(structureTree->array[1].word,"test")==0 && structureTree->array[1].nextWordArray!=NULL && structureTree->array[1].isFinal==true);
	assert(strcmp(structureTree->array[2].word,"this")==0 && structureTree->array[2].nextWordArray!=NULL && structureTree->array[2].isFinal==false);
	
	arrayOfStructs* nextArray = structureTree->array[2].nextWordArray; 
	assert(nextArray->length==10 && nextArray->position==2 && nextArray->array!=NULL);
	
	assert(strcmp(nextArray->array[0].word,"cat")==0 && nextArray->array[0].nextWordArray!=NULL && nextArray->array[0].isFinal==true);
	assert(strcmp(nextArray->array[1].word,"dog")==0 && nextArray->array[1].nextWordArray!=NULL && nextArray->array[1].isFinal==true);
	
	deleteArray(structureTree);
	//assert(nextArray==NULL);
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
	//checkItemExists* getPosition = malloc(sizeof(checkItemExists));									//HERE WAS LEAK
	checkItemExists* getPosition = insertionSort(array_of_str,item,array_of_str->position);			//array is empty		
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

void test_deletionSort(){
	arrayOfStructs* array_of_str = malloc(sizeof(arrayOfStructs));
	initializeArray(array_of_str);
	
	
	dataNode *item = malloc(sizeof(dataNode));
	item->word = malloc((strlen("hello")+1) * sizeof(char));
	strcpy(item->word,"hello");																
	assert(insertionSort(array_of_str,item,array_of_str->position)!=NULL);
	
	deletionSort(array_of_str,0,array_of_str->position);		//delete only element
	assert(array_of_str!=NULL);
	assert(array_of_str->array!=NULL);
	assert(array_of_str->length==10);
	assert(array_of_str->position==-1);
	
	deleteDataNode(item);
	free(item);
	item=NULL;
	
	array_of_str->position=0;
	//printf("pos1 %d\n",array_of_str->position);
	
	
	item = malloc(sizeof(dataNode));
	item->word = malloc((strlen("hello")+1) * sizeof(char));
	strcpy(item->word,"hello");																
	assert(insertionSort(array_of_str,item,array_of_str->position)!=NULL);		//add hello
	/*deleteDataNode(item);
	free(item);
	item=NULL;*/
	
	array_of_str->position++;
	
	
	dataNode* item2 = malloc(sizeof(dataNode));
	item2->word = malloc((strlen("test")+1) * sizeof(char));
	strcpy(item2->word,"test");																
	insertionSort(array_of_str,item2,array_of_str->position);	//add test
	
	array_of_str->position++;

	
	dataNode* item3 = malloc(sizeof(dataNode));
	item3->word = malloc((strlen("the")+1) * sizeof(char));
	strcpy(item3->word,"the");																
	assert(insertionSort(array_of_str,item3,array_of_str->position)!=NULL);		//add the
	
	array_of_str->position++;
	
	dataNode* item4 = malloc(sizeof(dataNode));
	item4->word = malloc((strlen("this")+1) * sizeof(char));
	strcpy(item4->word,"this");						
	assert(insertionSort(array_of_str,item4,array_of_str->position)!=NULL);		//add this
	
	array_of_str->position++;
	
	
	deletionSort(array_of_str,3,array_of_str->position);		//from hello, test, the, this delete this
	
	
	assert(array_of_str!=NULL);
	assert(array_of_str->array!=NULL);
	assert(array_of_str->length==10);
	assert(array_of_str->position==3);
	
	assert(strcmp(array_of_str->array[0].word,"hello")==0);
	assert(strcmp(array_of_str->array[1].word,"test")==0);
	assert(strcmp(array_of_str->array[2].word,"the")==0);
	
	dataNode* item5 = malloc(sizeof(dataNode));
	item5->word = malloc((strlen("this")+1) * sizeof(char));
	strcpy(item5->word,"this");																
	assert(insertionSort(array_of_str,item5,array_of_str->position)!=NULL);		//add this
	
	array_of_str->position++;
	
	deletionSort(array_of_str,1,array_of_str->position);		//from hello, test, the, this delete test
	assert(array_of_str!=NULL);
	assert(array_of_str->array!=NULL);
	assert(array_of_str->length==10);
	assert(array_of_str->position==3);
	assert(strcmp(array_of_str->array[0].word,"hello")==0);	
	assert(strcmp(array_of_str->array[1].word,"the")==0);
	assert(strcmp(array_of_str->array[2].word,"this")==0);
	
	//deleteArray(array_of_str);
	

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
	
	search_ngram(array_of_str, arrayOfWords, noOfWords);
	
	
	for(int i=0;i<3;i++){												//free 
		free(arrayOfWords[i]);
		arrayOfWords[i] = NULL;
	}
	free(arrayOfWords);
	arrayOfWords = NULL;
	deleteArray(array_of_str);
	array_of_str = NULL;
	
	printf("End of testing search_ngram\n");
}

void test_delete_ngram(){
	
}
/*int delete_ngram(arrayOfStructs* array_of_structs, char** arrayOfWords, int noOfWords){
	//printf("<-------------------------DELETE STARTING------------------------->\n");
	arrayOfStructs* tempArray = array_of_structs;
	stack* myStack = malloc(sizeof(stack));
	initializeStack(myStack);
	for(int i=0; i<noOfWords; i++){
	
		//printArray(tempArray,tempArray->position-1);
		//printf("Must delete word '%s'\n",arrayOfWords[i]);
		int position = tempArray->position;
		
		dataNode* tempElement = malloc(sizeof(dataNode));
		tempElement->word= (char*)malloc((strlen(arrayOfWords[i])+1) * sizeof(char));
		strcpy(tempElement->word,arrayOfWords[i]);	
		
		
		//find out if word exists in array and if it does return position
		checkItemExists* getPosition = binarySearch(tempArray, tempElement,0 ,tempArray->position);
		if(	getPosition->exists==true){
			//printf("found inside array\n");
			push(myStack, getPosition->position);
		}
		else{										//element was not found inside array so it can not be deleted	
			//printf("element '%s' was not found \n",tempElement->word);
			deleteDataNode(tempElement);
			free(getPosition);
			getPosition = NULL;
			break;			
		}
			
		
		//printArrayFinalWords(tempArray,tempArray->position-1);
		
		tempArray = tempArray->array[getPosition->position].nextWordArray;
		if(tempArray == NULL)
		{
			deleteDataNode(tempElement);
			free(getPosition);
			getPosition = NULL;
			break;			
		}
		deleteDataNode(tempElement);
		free(getPosition);
		getPosition = NULL;
		
	}
	
	
	//displayStack(myStack);
	
	//delete elements in positions of myStack
	
	
	//while stack not empty
	while(!isEmpty(myStack)){
		//root of trie
		tempArray = array_of_structs;
		//displayStack(myStack);
		
		//get last element of stack
		for(int i=0;i <= myStack->top ;i++){
			if(tempArray != NULL){
				tempArray = tempArray->array[myStack->positionsToDelete[i]].nextWordArray;
			}
		}

		
		if(tempArray->array[myStack->positionsToDelete[myStack->top]].nextWordArray == NULL){			//hasnt got children
			deletionSort(tempArray,myStack->positionsToDelete[myStack->top], tempArray->position);
		}else{																						//has children
			tempArray->array[myStack->positionsToDelete[myStack->top]].isFinal = false;
			break;																			//final word -> not final and exit
		}

		//pop top item
		int popElem = pop(myStack);
		if(!popElem){
			printf("Error with popping element from stack.\n");
			break;
		}
		
	}
	
	deleteStack(myStack);
	free(myStack);
	myStack = NULL;
	
	//printf("<-------------------------DELETE ENDING------------------------->\n");
	
	return 1;


}
*/



















