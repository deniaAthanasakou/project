#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "test.h"
#include "stack.h"
#include "auxMethods.h"


void testAllFunctions(){		//calls all testFunctions
	
	
	test_initializeStack();
	test_deleteStack();
	test_doubleLengthOfStack();
	test_push();
	test_isEmpty();
	test_pop();
	
	test_deleteArrayOfWords();
	
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
































