#include <stdio.h>
#include <stdlib.h>
#include "CuTestStack.h"


void TestInitializeStack(CuTest *tc) {
    stack* myStack = malloc(sizeof(stack));
  	initializeStack(myStack);

    CuAssertIntEquals(tc,-1,myStack->top); 
    CuAssertIntEquals(tc,10,myStack->length); 
    CuAssertPtrNotNull(tc,myStack->positionsToDelete);
    
    deleteStack(myStack);
  	free(myStack);
	myStack = NULL;
}

void TestDeleteStack(CuTest *tc) {
    stack* myStack=NULL;
	deleteStack(myStack);			//delete when stack is null
  	CuAssertPtrEquals(tc,NULL,myStack); 
  		
	myStack = malloc(sizeof(stack));
	myStack->positionsToDelete=NULL;
	deleteStack(myStack);			//delete when stack is not initialized
	free(myStack);
	myStack = NULL;
  	CuAssertPtrEquals(tc,NULL,myStack); 
  	
  	myStack = malloc(sizeof(stack));
  	initializeStack(myStack);
  	deleteStack(myStack);			//delete when stack is not null and is initialized
  	free(myStack);
	myStack = NULL;
  	CuAssertPtrEquals(tc,NULL,myStack); 
}

void TestDoubleLengthOfStack(CuTest *tc){
	stack* myStack = malloc(sizeof(stack));
  	initializeStack(myStack);
  	doubleLengthOfStack(myStack);
  	
  	CuAssertIntEquals(tc,-1,myStack->top); 
    CuAssertIntEquals(tc,20,myStack->length); 
    CuAssertPtrNotNull(tc,myStack->positionsToDelete);
    
  	deleteStack(myStack);
  	free(myStack);
	myStack = NULL;
}

void TestPush(CuTest *tc){
	stack* myStack = malloc(sizeof(stack));
  	initializeStack(myStack);

  	push(myStack,0);		//test if one element can be pushed into the stack when stack is empty
  	CuAssertIntEquals(tc,0,myStack->top); 
    CuAssertIntEquals(tc,0,myStack->positionsToDelete[0]); 
  	
  	for(int i=1; i<=10; i++){	//push 10 elements so that stack will be doubled
  		push(myStack,i);
  	}
  	
  	CuAssertIntEquals(tc,10,myStack->top); 
    CuAssertIntEquals(tc,20,myStack->length); 

  	for(int i=0; i<=10; i++){
  		CuAssertIntEquals(tc,i,myStack->positionsToDelete[i]); 
  	}
  	deleteStack(myStack);
  	free(myStack);
	myStack = NULL;
  	
}


void TestIsEmpty(CuTest *tc){
	stack* myStack = malloc(sizeof(stack));
  	initializeStack(myStack);
  	
  	CuAssertTrue(tc, isEmpty(myStack));
  	
  	push(myStack, 0);
  	
  	CuAssertTrue(tc, !isEmpty(myStack));
  	
  	deleteStack(myStack);
  	free(myStack);
	myStack = NULL;
}

void TestPop(CuTest *tc){
	stack* myStack = malloc(sizeof(stack));
  	initializeStack(myStack);
  	
  	CuAssertIntEquals(tc,-1,myStack->top); 
  	CuAssertTrue(tc, !pop(myStack));	//pop when stack is empty
  	CuAssertIntEquals(tc,-1,myStack->top); 
  	
  	push(myStack, 0);
  	
  	CuAssertIntEquals(tc,0,myStack->top); 
  	CuAssertTrue(tc, pop(myStack));			//pop when stack is not empty
  	CuAssertIntEquals(tc,-1,myStack->top); 
  	
  	deleteStack(myStack);
  	free(myStack);
	myStack = NULL;
}


CuSuite* StackGetSuite() {		//adding TestStack Functions into suite
    CuSuite* suite = CuSuiteNew();
    
    SUITE_ADD_TEST(suite, TestInitializeStack);
    SUITE_ADD_TEST(suite, TestDeleteStack);
    SUITE_ADD_TEST(suite, TestDoubleLengthOfStack);
    SUITE_ADD_TEST(suite, TestPush);
    SUITE_ADD_TEST(suite, TestIsEmpty);
    SUITE_ADD_TEST(suite, TestPop);
    
    return suite;
}



