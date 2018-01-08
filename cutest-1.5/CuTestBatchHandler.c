#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "CuTestBatchHandler.h"


void TestInitializeInstructionArray(CuTest *tc){

	arrayOfInstructions* arrayOfInstr = initializeInstructionArray();
	CuAssertPtrNotNull(tc,arrayOfInstr);
	CuAssertIntEquals(tc,10,arrayOfInstr->length); 
    CuAssertPtrNotNull(tc,arrayOfInstr->array);
    CuAssertIntEquals(tc,0,arrayOfInstr->position); 
    for(int i = 0; i< arrayOfInstr->length; i++){
    	assert(arrayOfInstr->array[i].type=='A');
		CuAssertPtrEquals(tc,NULL,arrayOfInstr->array[i].ngram); 
		CuAssertIntEquals(tc,0,arrayOfInstr->array[i].num); 
		CuAssertIntEquals(tc,-1,arrayOfInstr->array[i].numForQ); 
	}
	
	destroyInstructionArray(arrayOfInstr);
}

void TestDoubleInstructionArray(CuTest *tc){
	arrayOfInstructions* arrayOfInstr = initializeInstructionArray();
	doubleInstructionArray(arrayOfInstr);
	CuAssertIntEquals(tc,20,arrayOfInstr->length); 
	CuAssertPtrNotNull(tc,arrayOfInstr->array);
	for(int i = 10; i< arrayOfInstr->length; i++){
    	assert(arrayOfInstr->array[i].type=='A');
		CuAssertPtrEquals(tc,NULL,arrayOfInstr->array[i].ngram); 
		CuAssertIntEquals(tc,0,arrayOfInstr->array[i].num); 
		CuAssertIntEquals(tc,-1,arrayOfInstr->array[i].numForQ); 
	}
	destroyInstructionArray(arrayOfInstr);
}



void TestInsertInstructionArray(CuTest *tc){
	arrayOfInstructions* arrayOfInstr = initializeInstructionArray();
	instruction* node = malloc(sizeof(instruction));
	initializeInstruction(node);
	
	CuAssertIntEquals(tc,0,arrayOfInstr->position); 
	
	insertInstructionArray(arrayOfInstr, node);									//insert unchanged node
	
	assert(arrayOfInstr->array[0].type=='A');
	CuAssertPtrEquals(tc,NULL,arrayOfInstr->array[0].ngram); 
	CuAssertIntEquals(tc,0,arrayOfInstr->array[0].num); 
	CuAssertIntEquals(tc,-1,arrayOfInstr->array[0].numForQ); 
	
	CuAssertIntEquals(tc,1,arrayOfInstr->position); 
	
	free(node);
	node=NULL;
	
	node = malloc(sizeof(instruction));
	initializeInstruction(node);
	node->type='D';
	node->ngram = malloc(10*sizeof(char));
	strcpy(node->ngram, "cat");
	node->num = 25;
	node->numForQ = 15;
	insertInstructionArray(arrayOfInstr, node);									//insert changed node
	
	CuAssertIntEquals(tc,2,arrayOfInstr->position); 
	
	assert(arrayOfInstr->array[1].type=='D');
	CuAssertPtrNotNull(tc,arrayOfInstr->array[1].ngram); 
	CuAssertStrEquals(tc,"cat",arrayOfInstr->array[1].ngram); 
	CuAssertIntEquals(tc,25,arrayOfInstr->array[1].num); 
	CuAssertIntEquals(tc,15,arrayOfInstr->array[1].numForQ); 
	
	
	free(node);
	node=NULL;
	
	
	for(int i = 2; i<15; i++){													//array will be doubled
	
		node = malloc(sizeof(instruction));
		initializeInstruction(node);
		node->type='D';
		node->ngram = malloc(10*sizeof(char));
		strcpy(node->ngram, "cat");
		node->num = 25;
		node->numForQ = 15;
		insertInstructionArray(arrayOfInstr, node);								//insert changed node
	
		assert(arrayOfInstr->array[i].type=='D');
		CuAssertPtrNotNull(tc,arrayOfInstr->array[i].ngram); 
		CuAssertStrEquals(tc,"cat",arrayOfInstr->array[i].ngram); 
		CuAssertIntEquals(tc,25,arrayOfInstr->array[i].num); 
		CuAssertIntEquals(tc,15,arrayOfInstr->array[i].numForQ); 
		
		CuAssertIntEquals(tc,i+1,arrayOfInstr->position); 
		free(node);
		node=NULL;
	}
	CuAssertIntEquals(tc,20,arrayOfInstr->length); 
	destroyInstructionArray(arrayOfInstr);
}


void TestInitializeInstruction(CuTest *tc){
	instruction* instr = malloc(sizeof(instruction));
	initializeInstruction(instr);
	assert(instr->type=='A');
	CuAssertPtrEquals(tc,NULL,instr->ngram); 
	CuAssertIntEquals(tc,0,instr->num); 
	CuAssertIntEquals(tc,-1,instr->numForQ); 
	destroyInstruction(instr);
	free(instr);
	instr = NULL;
}
void TestDestroyInstruction(CuTest *tc){
	instruction* instr = malloc(sizeof(instruction));
	initializeInstruction(instr);
	destroyInstruction(instr);						//not changed instruction
	
	
	assert(instr->type=='A');
	CuAssertPtrEquals(tc,NULL,instr->ngram); 
	CuAssertIntEquals(tc,0,instr->num); 
	CuAssertIntEquals(tc,-1,instr->numForQ); 
	
	free(instr);
	instr = NULL;
	
	instr = malloc(sizeof(instruction));
	initializeInstruction(instr);
	
	instr->type='D';
	instr->ngram = malloc(10*sizeof(char));
	strcpy(instr->ngram, "cat");
	instr->num = 25;
	instr->numForQ = 15;
	
	destroyInstruction(instr);						//changed instruction
	
	assert(instr->type=='A');
	CuAssertPtrEquals(tc,NULL,instr->ngram); 
	CuAssertIntEquals(tc,0,instr->num); 
	CuAssertIntEquals(tc,-1,instr->numForQ); 
	
	free(instr);
	instr = NULL;
	
}



void TestRearrange_MoveUpArray(CuTest *tc){
	arrayOfInstructions* arrayOfInstr = initializeInstructionArray();
	
	instruction* node = malloc(sizeof(instruction));
	initializeInstruction(node);
	node->type='D';
	node->ngram = malloc(10*sizeof(char));
	strcpy(node->ngram, "cat1");
	node->num = 25;
	node->numForQ = 15;
	insertInstructionArray(arrayOfInstr, node);	
	free(node);
	node=NULL;
	
	node = malloc(sizeof(instruction));
	initializeInstruction(node);
	node->type='A';
	node->ngram = malloc(10*sizeof(char));
	strcpy(node->ngram, "cat2");
	node->num = 26;
	node->numForQ = 16;
	insertInstructionArray(arrayOfInstr, node);	
	free(node);
	node=NULL;
	
	node = malloc(sizeof(instruction));
	initializeInstruction(node);
	node->type='Q';
	node->ngram = malloc(10*sizeof(char));
	strcpy(node->ngram, "cat3");
	node->num = 27;
	node->numForQ = 17;
	insertInstructionArray(arrayOfInstr, node);	
	free(node);
	node=NULL;
	
	
	
	rearrangeArray(arrayOfInstr);		//will not change
	
	assert(arrayOfInstr->array[0].type=='D');
	CuAssertPtrNotNull(tc,arrayOfInstr->array[0].ngram); 
	CuAssertStrEquals(tc,"cat1",arrayOfInstr->array[0].ngram); 
	CuAssertIntEquals(tc,25,arrayOfInstr->array[0].num); 
	CuAssertIntEquals(tc,15,arrayOfInstr->array[0].numForQ);
	
	assert(arrayOfInstr->array[1].type=='A');
	CuAssertPtrNotNull(tc,arrayOfInstr->array[1].ngram); 
	CuAssertStrEquals(tc,"cat2",arrayOfInstr->array[1].ngram); 
	CuAssertIntEquals(tc,26,arrayOfInstr->array[1].num); 
	CuAssertIntEquals(tc,16,arrayOfInstr->array[1].numForQ);
	
	assert(arrayOfInstr->array[2].type=='Q');
	CuAssertPtrNotNull(tc,arrayOfInstr->array[2].ngram); 
	CuAssertStrEquals(tc,"cat3",arrayOfInstr->array[2].ngram); 
	CuAssertIntEquals(tc,27,arrayOfInstr->array[2].num); 
	CuAssertIntEquals(tc,17,arrayOfInstr->array[2].numForQ);
	
	
	
	destroyInstructionArray(arrayOfInstr);
	arrayOfInstr = initializeInstructionArray();
	
	node = malloc(sizeof(instruction));
	initializeInstruction(node);
	node->type='D';
	node->ngram = malloc(10*sizeof(char));
	strcpy(node->ngram, "cat1");
	node->num = 25;
	node->numForQ = 15;
	insertInstructionArray(arrayOfInstr, node);	
	free(node);
	node=NULL;
	
	node = malloc(sizeof(instruction));
	initializeInstruction(node);
	node->type='Q';
	node->ngram = malloc(10*sizeof(char));
	strcpy(node->ngram, "cat2");
	node->num = 26;
	node->numForQ = 16;
	insertInstructionArray(arrayOfInstr, node);	
	free(node);
	node=NULL;
	
	node = malloc(sizeof(instruction));
	initializeInstruction(node);
	node->type='Q';
	node->ngram = malloc(10*sizeof(char));
	strcpy(node->ngram, "cat3");
	node->num = 27;
	node->numForQ = 17;
	insertInstructionArray(arrayOfInstr, node);	
	free(node);
	node=NULL;
	
	node = malloc(sizeof(instruction));
	initializeInstruction(node);
	node->type='A';
	node->ngram = malloc(10*sizeof(char));
	strcpy(node->ngram, "cat4");
	node->num = 28;
	node->numForQ = 18;
	insertInstructionArray(arrayOfInstr, node);	
	free(node);
	node=NULL;
	
	node = malloc(sizeof(instruction));
	initializeInstruction(node);
	node->type='Q';
	node->ngram = malloc(10*sizeof(char));
	strcpy(node->ngram, "cat5");
	node->num = 29;
	node->numForQ = 19;
	insertInstructionArray(arrayOfInstr, node);	
	free(node);
	node=NULL;
	
	rearrangeArray(arrayOfInstr);	//D,A,Q,Q,Q	
	
	assert(arrayOfInstr->array[0].type=='D');
	CuAssertPtrNotNull(tc,arrayOfInstr->array[0].ngram); 
	CuAssertStrEquals(tc,"cat1",arrayOfInstr->array[0].ngram); 
	CuAssertIntEquals(tc,25,arrayOfInstr->array[0].num); 
	CuAssertIntEquals(tc,15,arrayOfInstr->array[0].numForQ);
	
	assert(arrayOfInstr->array[1].type=='A');
	CuAssertPtrNotNull(tc,arrayOfInstr->array[1].ngram); 
	CuAssertStrEquals(tc,"cat4",arrayOfInstr->array[1].ngram); 
	CuAssertIntEquals(tc,28,arrayOfInstr->array[1].num); 
	CuAssertIntEquals(tc,18,arrayOfInstr->array[1].numForQ);
	
	assert(arrayOfInstr->array[2].type=='Q');
	CuAssertPtrNotNull(tc,arrayOfInstr->array[2].ngram); 
	CuAssertStrEquals(tc,"cat2",arrayOfInstr->array[2].ngram); 
	CuAssertIntEquals(tc,26,arrayOfInstr->array[2].num); 
	CuAssertIntEquals(tc,16,arrayOfInstr->array[2].numForQ);
	
	assert(arrayOfInstr->array[3].type=='Q');
	CuAssertPtrNotNull(tc,arrayOfInstr->array[3].ngram); 
	CuAssertStrEquals(tc,"cat3",arrayOfInstr->array[3].ngram); 
	CuAssertIntEquals(tc,27,arrayOfInstr->array[3].num); 
	CuAssertIntEquals(tc,17,arrayOfInstr->array[3].numForQ);
	
	assert(arrayOfInstr->array[4].type=='Q');
	CuAssertPtrNotNull(tc,arrayOfInstr->array[4].ngram); 
	CuAssertStrEquals(tc,"cat5",arrayOfInstr->array[4].ngram); 
	CuAssertIntEquals(tc,29,arrayOfInstr->array[4].num); 
	CuAssertIntEquals(tc,19,arrayOfInstr->array[4].numForQ);
	
	
	destroyInstructionArray(arrayOfInstr);
	arrayOfInstr = initializeInstructionArray();
	
	
	node = malloc(sizeof(instruction));
	initializeInstruction(node);
	node->type='Q';
	node->ngram = malloc(10*sizeof(char));
	strcpy(node->ngram, "cat1");
	node->num = 25;
	node->numForQ = 15;
	insertInstructionArray(arrayOfInstr, node);	
	free(node);
	node=NULL;
	
	node = malloc(sizeof(instruction));
	initializeInstruction(node);
	node->type='A';
	node->ngram = malloc(10*sizeof(char));
	strcpy(node->ngram, "cat2");
	node->num = 26;
	node->numForQ = 16;
	insertInstructionArray(arrayOfInstr, node);	
	free(node);
	node=NULL;
	
	rearrangeArray(arrayOfInstr);			//A,Q
	
	assert(arrayOfInstr->array[0].type=='A');
	CuAssertPtrNotNull(tc,arrayOfInstr->array[0].ngram); 
	CuAssertStrEquals(tc,"cat2",arrayOfInstr->array[0].ngram); 
	CuAssertIntEquals(tc,26,arrayOfInstr->array[0].num); 
	CuAssertIntEquals(tc,16,arrayOfInstr->array[0].numForQ);
	
	assert(arrayOfInstr->array[1].type=='Q');
	CuAssertPtrNotNull(tc,arrayOfInstr->array[1].ngram); 
	CuAssertStrEquals(tc,"cat1",arrayOfInstr->array[1].ngram); 
	CuAssertIntEquals(tc,25,arrayOfInstr->array[1].num); 
	CuAssertIntEquals(tc,15,arrayOfInstr->array[1].numForQ);
	
	destroyInstructionArray(arrayOfInstr);
}

void TestInitializeInstrStaticArr(CuTest *tc){
	arrayOfInstrStatic* arrayOfInstr = initializeInstrStaticArr();
	CuAssertPtrNotNull(tc,arrayOfInstr);
	CuAssertIntEquals(tc,10,arrayOfInstr->length); 
    CuAssertPtrNotNull(tc,arrayOfInstr->array);
    CuAssertIntEquals(tc,0,arrayOfInstr->position); 
    for(int i = 0; i< arrayOfInstr->length; i++){
    	assert(arrayOfInstr->array[i].type=='Q');
		CuAssertPtrEquals(tc,NULL,arrayOfInstr->array[i].ngram); 
		CuAssertIntEquals(tc,-1,arrayOfInstr->array[i].num); 
	}
	
	destroyInstrStaticArray(arrayOfInstr);
}

void TestInitializeInstrStatic(CuTest *tc){
	instructionStatic* instr = malloc(sizeof(instructionStatic));
	initializeInstrStatic(instr);
	assert(instr->type=='Q');
	CuAssertPtrEquals(tc,NULL,instr->ngram); 
	CuAssertIntEquals(tc,-1,instr->num); 
	destroyInstructionStatic(instr);
	free(instr);
	instr = NULL;
}

void TestDoubleInstrStaticArray(CuTest *tc){
	arrayOfInstrStatic* arrayOfInstr = initializeInstrStaticArr();
	doubleInstrStaticArray(arrayOfInstr);
	CuAssertIntEquals(tc,20,arrayOfInstr->length); 
	CuAssertPtrNotNull(tc,arrayOfInstr->array);
	for(int i = 10; i< arrayOfInstr->length; i++){
    	assert(arrayOfInstr->array[i].type=='Q');
		CuAssertPtrEquals(tc,NULL,arrayOfInstr->array[i].ngram); 
		CuAssertIntEquals(tc,-1,arrayOfInstr->array[i].num); 
	}
	destroyInstrStaticArray(arrayOfInstr);
}

void TestInsertInstrStaticArray(CuTest *tc){
	arrayOfInstrStatic* arrayOfInstr = initializeInstrStaticArr();
	instructionStatic* node = malloc(sizeof(instructionStatic));
	initializeInstrStatic(node);
	
	CuAssertIntEquals(tc,0,arrayOfInstr->position); 
	
	insertInstrStaticArray(arrayOfInstr, node);									//insert unchanged node
	
	assert(arrayOfInstr->array[0].type=='Q');
	CuAssertPtrEquals(tc,NULL,arrayOfInstr->array[0].ngram); 
	CuAssertIntEquals(tc,-1,arrayOfInstr->array[0].num); 
	
	CuAssertIntEquals(tc,1,arrayOfInstr->position); 
	
	free(node);
	node=NULL;
	
	node = malloc(sizeof(instructionStatic));
	initializeInstrStatic(node);
	node->type='Q';
	node->ngram = malloc(10*sizeof(char));
	strcpy(node->ngram, "cat");
	node->num = 25;
	insertInstrStaticArray(arrayOfInstr, node);									//insert changed node
	
	CuAssertIntEquals(tc,2,arrayOfInstr->position); 
	
	assert(arrayOfInstr->array[1].type=='Q');
	CuAssertPtrNotNull(tc,arrayOfInstr->array[1].ngram); 
	CuAssertStrEquals(tc,"cat",arrayOfInstr->array[1].ngram); 
	CuAssertIntEquals(tc,25,arrayOfInstr->array[1].num); 
	
	
	free(node);
	node=NULL;
	
	
	for(int i = 2; i<15; i++){													//array will be doubled
	
		node = malloc(sizeof(instructionStatic));
		initializeInstrStatic(node);
		node->type='Q';
		node->ngram = malloc(10*sizeof(char));
		strcpy(node->ngram, "cat");
		node->num = 25;
		insertInstrStaticArray(arrayOfInstr, node);								//insert changed node
	
		assert(arrayOfInstr->array[i].type=='Q');
		CuAssertPtrNotNull(tc,arrayOfInstr->array[i].ngram); 
		CuAssertStrEquals(tc,"cat",arrayOfInstr->array[i].ngram); 
		CuAssertIntEquals(tc,25,arrayOfInstr->array[i].num); 
		CuAssertIntEquals(tc,i+1,arrayOfInstr->position); 
		
		free(node);
		node=NULL;
	}
	CuAssertIntEquals(tc,20,arrayOfInstr->length); 
	destroyInstrStaticArray(arrayOfInstr);
}

void TestDestroyInstructionStatic(CuTest *tc){
	instructionStatic* instr = malloc(sizeof(instructionStatic));
	initializeInstrStatic(instr);
	destroyInstructionStatic(instr);						//not changed instruction
	
	
	assert(instr->type=='Q');
	CuAssertPtrEquals(tc,NULL,instr->ngram); 
	CuAssertIntEquals(tc,-1,instr->num); 
	
	free(instr);
	instr = NULL;
	
	instr = malloc(sizeof(instructionStatic));
	initializeInstrStatic(instr);
	
	instr->type='Q';
	instr->ngram = malloc(10*sizeof(char));
	strcpy(instr->ngram, "cat");
	instr->num = 25;
	
	destroyInstructionStatic(instr);						//changed instruction
	
	assert(instr->type=='Q');
	CuAssertPtrEquals(tc,NULL,instr->ngram); 
	CuAssertIntEquals(tc,-1,instr->num); 
	
	free(instr);
	instr = NULL;
}


CuSuite* BatchHandlerGetSuite() {		//adding TestBatchHandler Functions into suite
    CuSuite* suite = CuSuiteNew();
    
    SUITE_ADD_TEST(suite, TestInitializeInstructionArray);
    SUITE_ADD_TEST(suite, TestDoubleInstructionArray);
    SUITE_ADD_TEST(suite, TestInsertInstructionArray);
    SUITE_ADD_TEST(suite, TestInitializeInstruction);
    SUITE_ADD_TEST(suite, TestDestroyInstruction);
    SUITE_ADD_TEST(suite, TestRearrange_MoveUpArray);
	SUITE_ADD_TEST(suite, TestInitializeInstrStaticArr);
	SUITE_ADD_TEST(suite, TestInitializeInstrStatic);
	SUITE_ADD_TEST(suite, TestDoubleInstrStaticArray);
	SUITE_ADD_TEST(suite, TestInsertInstrStaticArray);
	SUITE_ADD_TEST(suite, TestDestroyInstructionStatic);
    
    return suite;
}

