#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "test.h"
#include "stack.h"


void testAllFunctions(){
//call all testFunctions
	
	test_initializeStack();
}

void test_initializeStack(){
	stack* myStack = malloc(sizeof(stack));
  	initializeStack(myStack);
  	assert(myStack->top==-1 && myStack->length==10 && myStack->positionsToDelete!=NULL);
}

/*void test_doubleLengthOfStack(){
	stack* myStack = malloc(sizeof(stack));
  	initializeStack(myStack);
  	assert(myStack->top==-1 && myStack->length==10 && myStack->positionsToDelete!=NULL);
}*/

