#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "CuTestAuxMethods.h"




void TestStringToArray(CuTest *tc){
	char* ngram=NULL;
	arrayWords* array = stringToArray(ngram);
	
	CuAssertPtrNotNull(tc,array);
	CuAssertIntEquals(tc,0,array->length);    
	CuAssertPtrNotNull(tc,array->words);
	 
	free(array->words);
	array->words = NULL;
	free(array);
	array=NULL;
	
	ngram=malloc((strlen("The fox is bad and the cat is good.")+1) * sizeof(char));
	strcpy(ngram,"The fox is bad and the cat is good.");
	array = stringToArray(ngram);
	
	CuAssertPtrNotNull(tc,array);
	CuAssertIntEquals(tc,9,array->length);    
	CuAssertPtrNotNull(tc,array->words);
	
	CuAssertStrEquals(tc,"The",array->words[0]); 
	CuAssertStrEquals(tc,"fox",array->words[1]); 
	CuAssertStrEquals(tc,"is",array->words[2]); 
	CuAssertStrEquals(tc,"bad",array->words[3]); 
	CuAssertStrEquals(tc,"and",array->words[4]); 
	CuAssertStrEquals(tc,"the",array->words[5]); 
	CuAssertStrEquals(tc,"cat",array->words[6]); 
	CuAssertStrEquals(tc,"is",array->words[7]); 
	CuAssertStrEquals(tc,"good.",array->words[8]); 
	
	
	free(array->words);
	array->words = NULL;
	free(array);
	array=NULL;
	
	free(ngram);
	ngram=NULL;
}


void TestInitialize(CuTest *tc){
	/*text inside test_initialize:
		test
		heterotransplantation
		this dog
		this cat
	*/

	HashTable* hashTable = createLinearHash(NOOFBUCKETS, NOOFCELLS);
	FILE * initFile;
	initFile = fopen ("testingFiles/test_initialize","r");
	int staticDynamic = 1;
	if (initFile!=NULL)
	{
		staticDynamic = initialize(initFile, hashTable);
		fclose (initFile);
	}
	
	CuAssertIntEquals(tc,0,staticDynamic);  			//static file
	
	CuAssertIntEquals(tc,4,hashTable->length);  
	CuAssertStrEquals(tc,"heterotransplantation",hashTable->buckets[0].cells[0].dynamicWord); 
	CuAssertPtrEquals(tc,NULL,hashTable->buckets[0].cells[0].nextWordArray);
	CuAssertTrue(tc, hashTable->buckets[0].cells[0].isFinal);
	CuAssertTrue(tc, hashTable->buckets[0].cells[0].isDynamic);
	CuAssertStrEquals(tc,"\0",hashTable->buckets[0].cells[0].word);  
	
	CuAssertStrEquals(tc,"test",hashTable->buckets[0].cells[1].word); 
	CuAssertPtrEquals(tc,NULL,hashTable->buckets[0].cells[1].nextWordArray);
	CuAssertTrue(tc, hashTable->buckets[0].cells[1].isFinal);
	CuAssertTrue(tc, !hashTable->buckets[0].cells[1].isDynamic);
	
	CuAssertStrEquals(tc,"this",hashTable->buckets[0].cells[2].word); 
	CuAssertPtrNotNull(tc,hashTable->buckets[0].cells[2].nextWordArray);
	CuAssertTrue(tc, !hashTable->buckets[0].cells[2].isFinal);
	CuAssertTrue(tc, !hashTable->buckets[0].cells[2].isDynamic);
	
	
	arrayOfStructs* nextArray =hashTable->buckets[0].cells[2].nextWordArray; 
	
	CuAssertIntEquals(tc,10,nextArray->length);  
	CuAssertIntEquals(tc,2,nextArray->position);      
	CuAssertPtrNotNull(tc,nextArray->array);
	
	CuAssertStrEquals(tc,"cat",nextArray->array[0].word); 
	CuAssertPtrEquals(tc,NULL,nextArray->array[0].nextWordArray);
	CuAssertTrue(tc, nextArray->array[0].isFinal);
	CuAssertTrue(tc, !nextArray->array[0].isDynamic);
	
	CuAssertStrEquals(tc,"dog",nextArray->array[1].word); 
	CuAssertPtrEquals(tc,NULL,nextArray->array[1].nextWordArray);
	CuAssertTrue(tc, nextArray->array[1].isFinal);
	CuAssertTrue(tc, !nextArray->array[1].isDynamic);
	
	
	destroyLinearHash(hashTable);
	hashTable=NULL;
	CuAssertPtrEquals(tc,NULL,hashTable);
}

void TestBinarySearch(CuTest* tc){

	arrayOfStructs* array_of_str = malloc(sizeof(arrayOfStructs));
	initializeArray(array_of_str);
	checkItemExists* getPosition;
	
	dataNode *item = malloc(sizeof(dataNode));
	initializeDataNode(item);
	getPosition = binarySearch(array_of_str,item,0,array_of_str->position,NULL, false);			//array is empty
	CuAssertIntEquals(tc,-1,getPosition->position);
	CuAssertTrue(tc,!getPosition->exists);
	
	free(getPosition);
	getPosition = NULL;
	
		
	insertString(item,"cat");										//existing word
	insertString(&array_of_str->array[0],"cat");
	array_of_str->position++;
	insertString(&array_of_str->array[1],"dog");
	array_of_str->position++;
	insertString(&array_of_str->array[2],"whatabeautifuldaywego");
	array_of_str->position++;
	
	getPosition = binarySearch(array_of_str,item,0,array_of_str->position,NULL, false);		//binarySeach cat
	CuAssertIntEquals(tc,0,getPosition->position);
	CuAssertTrue(tc,getPosition->exists);
	
	free(getPosition);
	getPosition = NULL;	
	
	deleteDataNode(item);
	free(item);
	item = NULL;
	
	item = malloc(sizeof(dataNode));
	initializeDataNode(item);
	insertString(item,"whatabeautifuldaywego");
	getPosition = binarySearch(array_of_str,item,0,array_of_str->position,NULL, true);
	CuAssertIntEquals(tc,2,getPosition->position);
	CuAssertTrue(tc,getPosition->exists);
		
	free(getPosition);
	getPosition = NULL;	
	
	deleteDataNode(item);
	free(item);
	item = NULL;
	
	item = malloc(sizeof(dataNode));
	initializeDataNode(item);
	insertString(item,"hello");	
	getPosition = binarySearch(array_of_str,item,0,array_of_str->position,NULL, true);
	CuAssertIntEquals(tc,2,getPosition->position);
	CuAssertTrue(tc,!getPosition->exists);
	
	free(getPosition);
	getPosition = NULL;
	
	deleteDataNode(item);
	free(item);
	item = NULL;
	deleteArray(array_of_str);
	array_of_str = NULL;
	
}

void TestInsertionSort(CuTest* tc){

	arrayOfStructs* array_of_str = malloc(sizeof(arrayOfStructs));
	initializeArray(array_of_str);
	dataNode* item = malloc(sizeof(dataNode));
	initializeDataNode(item);

	insertString(item,"cat");

	array_of_str->array[0]= *item;
	array_of_str->position++;
	
	

	insertString(item,"dog");    
	array_of_str->array[1]= *item;
	array_of_str->position++;
	
	
	checkItemExists* getPosition = insertionSort(array_of_str,item,array_of_str->position);		//try to insert dog
	CuAssertTrue(tc,getPosition->position!=-1);
	CuAssertTrue(tc,getPosition->exists);
	
	free(getPosition);
	getPosition = NULL;
	
	
	insertString(item,"hello"); 
	
	getPosition = insertionSort(array_of_str,item,array_of_str->position);
	array_of_str->position++;               //non existing word
	CuAssertIntEquals(tc,2,getPosition->position);
	CuAssertTrue(tc,!getPosition->exists);
	
	free(getPosition);
	getPosition = NULL;
	
	
	insertString(item,"antidisestablishmentarianism");      
	
	getPosition = insertionSort(array_of_str,item,array_of_str->position);
	array_of_str->position++;               	//non existing word

	CuAssertIntEquals(tc,0,getPosition->position);
	CuAssertTrue(tc,!getPosition->exists);
	CuAssertStrEquals(tc,"antidisestablishmentarianism",array_of_str->array[0].dynamicWord);
	CuAssertStrEquals(tc,"cat",array_of_str->array[1].word);
	CuAssertStrEquals(tc,"dog",array_of_str->array[2].word);
	CuAssertStrEquals(tc,"hello",array_of_str->array[3].word);
	
	free(getPosition);
	getPosition = NULL;
	
	int lengthBefore = array_of_str->length;
	
	
	//add in order to double array
	//5th
	insertString(item,"hi");      
	
	getPosition = insertionSort(array_of_str,item,array_of_str->position);
	array_of_str->position++;
	
	free(getPosition);
	getPosition = NULL;
	
	//6th
	
	insertString(item,"testing");     
	
	getPosition = insertionSort(array_of_str,item,array_of_str->position);
	array_of_str->position++;
	
	free(getPosition);
	getPosition = NULL;
	
	//7th
	insertString(item,"before");  
	
	getPosition = insertionSort(array_of_str,item,array_of_str->position);
	array_of_str->position++;
	
	free(getPosition);
	getPosition = NULL;
	
	//8th
	insertString(item,"those");  
	
	getPosition = insertionSort(array_of_str,item,array_of_str->position);
	array_of_str->position++;
	
	free(getPosition);
	getPosition = NULL;
	
	//9th
	insertString(item,"what");  
	
	getPosition = insertionSort(array_of_str,item,array_of_str->position);
	array_of_str->position++;
	
	free(getPosition);
	getPosition = NULL;
	
	//10th
	insertString(item,"myy");  
	
	getPosition = insertionSort(array_of_str,item,array_of_str->position);
	array_of_str->position++;
	
	free(getPosition);
	getPosition = NULL;
	
	//check if array has been doubled
	
	CuAssertIntEquals(tc,2*lengthBefore,array_of_str->length);
	
	deleteDataNode(item);
	free(item);
	item=NULL;
	
	deleteArray(array_of_str);
	array_of_str = NULL;
	

}

void TestExecuteQueryFile_ExecuteDynamicArray(CuTest *tc){
	
	int executeQueryFile(FILE* file, HashTable* hashTable, int staticDynamic);
	HashTable* hashTable = NULL;
	int staticDynamic = 1;
	
	printf("Start of Testing executeQueryFile\n");
	FILE * queryFile;
	
	queryFile = fopen ("testingFiles/correct1batch","r");
	hashTable = createLinearHash(NOOFBUCKETS, NOOFCELLS);
	CuAssertIntEquals(tc,1,executeQueryFile(queryFile,hashTable,staticDynamic));		//correct file
	fclose (queryFile);
	hashTable = NULL;
	CuAssertPtrEquals(tc,NULL,hashTable);

	queryFile = fopen ("testingFiles/correctMultipleBatches","r");
	hashTable = createLinearHash(NOOFBUCKETS, NOOFCELLS);
	CuAssertIntEquals(tc,1,executeQueryFile(queryFile,hashTable,staticDynamic));					//correct file with multiple batches
	fclose (queryFile);
	hashTable = NULL;
	CuAssertPtrEquals(tc,NULL,hashTable);

	queryFile = fopen ("testingFiles/incorrectNoF","r");
	hashTable = createLinearHash(NOOFBUCKETS, NOOFCELLS);
	CuAssertIntEquals(tc,0,executeQueryFile(queryFile,hashTable,staticDynamic));				//file does not end with F
	fclose (queryFile);
	hashTable = NULL;
	CuAssertPtrEquals(tc,NULL,hashTable);
	
	queryFile = fopen ("testingFiles/IncorrectLetter","r");
	hashTable = createLinearHash(NOOFBUCKETS, NOOFCELLS);
	CuAssertIntEquals(tc,0,executeQueryFile(queryFile,hashTable,staticDynamic));					//query of file starts with sth else other than A,D,Q,F
	fclose (queryFile);
	hashTable = NULL;
	CuAssertPtrEquals(tc,NULL,hashTable);
	
	staticDynamic = 0;
	queryFile = fopen ("testingFiles/correctStaticFile","r");
	hashTable = createLinearHash(NOOFBUCKETS, NOOFCELLS);
	CuAssertIntEquals(tc,1,executeQueryFile(queryFile,hashTable,staticDynamic));				//query of file starts with sth else other than A,D,Q,F
	fclose (queryFile);
	hashTable = NULL;
	CuAssertPtrEquals(tc,NULL,hashTable);
	
	printf("End of Testing executeQueryFile\n");

}



void TestDeletionSort(CuTest *tc){

	arrayOfStructs* array_of_str = NULL;
	HashTable* hashTable = createLinearHash(NOOFBUCKETS, NOOFCELLS);
	
	char** item = malloc(2*sizeof(char*));
	item[0] = malloc((strlen("well")+1) * sizeof(char));
	strcpy(item[0],"well");
	item[1] = malloc((strlen("hello")+1) * sizeof(char));
	strcpy(item[1],"hello");																
	insert_ngram(hashTable, item, 2,0);							//add 'hello'
	
	array_of_str = hashTable->buckets[3].cells[0].nextWordArray;
	deletionSort(array_of_str,0,array_of_str->position);		//delete only element
	CuAssertPtrNotNull(tc,array_of_str);
	CuAssertPtrNotNull(tc,array_of_str->array);
	CuAssertIntEquals(tc,10,array_of_str->length);	
	CuAssertIntEquals(tc,0,array_of_str->position);	
	
	for(int i=0; i<2; i++){
		free(item[i]);
		item[i]=NULL;
	}
	free(item);
	item=NULL;
	
	item = malloc(2*sizeof(char*));
	item[0] = malloc((strlen("well")+1) * sizeof(char));
	strcpy(item[0],"well");
	item[1] = malloc((strlen("hello")+1) * sizeof(char));
	strcpy(item[1],"hello");																
	insert_ngram(hashTable, item, 2,0);							//add 'hello'
	
	for(int i=0; i<2; i++){
		free(item[i]);
		item[i]=NULL;
	}
	free(item);
	item=NULL;													
	
	item = malloc(2*sizeof(char*));
	item[0] = malloc((strlen("well")+1) * sizeof(char));
	strcpy(item[0],"well");
	item[1] = malloc((strlen("heterotransplantation")+1) * sizeof(char));
	strcpy(item[1],"heterotransplantation");																									
	insert_ngram(hashTable, item, 2,2);							//add 'heterotransplantation'
	
	for(int i=0; i<2; i++){
		free(item[i]);
		item[i]=NULL;
	}
	free(item);
	item=NULL;		
	
	item = malloc(2*sizeof(char*));
	item[0] = malloc((strlen("well")+1) * sizeof(char));
	strcpy(item[0],"well");
	item[1] = malloc((strlen("the")+1) * sizeof(char));
	strcpy(item[1],"the");																									
	insert_ngram(hashTable, item, 2,100);							//add 'the'
	
	for(int i=0; i<2; i++){
		free(item[i]);
		item[i]=NULL;
	}
	free(item);
	item=NULL;		
	
	item = malloc(2*sizeof(char*));
	item[0] = malloc((strlen("well")+1) * sizeof(char));
	strcpy(item[0],"well");
	item[1] = malloc((strlen("this")+1) * sizeof(char));
	strcpy(item[1],"this");																									
	insert_ngram(hashTable, item, 2,145);							//add 'this'
	
	for(int i=0; i<2; i++){
		free(item[i]);
		item[i]=NULL;
	}
	free(item);
	item=NULL;	

	array_of_str = hashTable->buckets[3].cells[0].nextWordArray;
	deletionSort(array_of_str,3,array_of_str->position);		//from hello, heterotransplantation, the, this delete this
	
	CuAssertPtrNotNull(tc,array_of_str);
	CuAssertPtrNotNull(tc,array_of_str->array);
	CuAssertIntEquals(tc,10,array_of_str->length);	
	CuAssertIntEquals(tc,3,array_of_str->position);	
	
	CuAssertStrEquals(tc,"hello",array_of_str->array[0].word); 
	
	CuAssertStrEquals(tc,"heterotransplantation",array_of_str->array[1].dynamicWord); 
	CuAssertTrue(tc, array_of_str->array[1].isDynamic);
	CuAssertStrEquals(tc,"\0",array_of_str->array[1].word); 
	CuAssertIntEquals(tc,2,array_of_str->array[1].additionVersion); 
	
	CuAssertStrEquals(tc,"the",array_of_str->array[2].word); 
	
	item = malloc(2*sizeof(char*));
	item[0] = malloc((strlen("well")+1) * sizeof(char));
	strcpy(item[0],"well");
	item[1] = malloc((strlen("this")+1) * sizeof(char));
	strcpy(item[1],"this");																									
	insert_ngram(hashTable, item, 2,7);							//add 'this'
	
	for(int i=0; i<2; i++){
		free(item[i]);
		item[i]=NULL;
	}
	free(item);
	item=NULL;	
	
	array_of_str = hashTable->buckets[3].cells[0].nextWordArray;
	deletionSort(array_of_str,1,array_of_str->position);		//from hello, heterotransplantation, the, this delete heterotransplantation
	CuAssertPtrNotNull(tc,array_of_str);
	CuAssertPtrNotNull(tc,array_of_str->array);
	CuAssertIntEquals(tc,10,array_of_str->length);	
	CuAssertIntEquals(tc,3,array_of_str->position);	
	
	CuAssertStrEquals(tc,"hello",array_of_str->array[0].word); 
	CuAssertIntEquals(tc,0,array_of_str->array[0].additionVersion); 
	CuAssertStrEquals(tc,"the",array_of_str->array[1].word); 
	CuAssertIntEquals(tc,100,array_of_str->array[1].additionVersion); 
	CuAssertStrEquals(tc,"this",array_of_str->array[2].word); 
	CuAssertIntEquals(tc,7,array_of_str->array[2].additionVersion); 
	

	destroyLinearHash(hashTable);
	hashTable=NULL;
	CuAssertPtrEquals(tc,NULL,hashTable);
}


void TestInsertString (CuTest *tc){
	dataNode* node = malloc(sizeof(dataNode));
	node->staticArray = NULL;
	node->staticArrayLength = 0;
	
	char* word = malloc (sizeof(char) *(strlen("hello")+1));
	strcpy(word,"hello");
	
	insertString(node, word);
	CuAssertStrEquals(tc,word,node->word);
	CuAssertIntEquals(tc, strlen(word) +1,node->noOfChars);
	CuAssertTrue(tc,!node->isDynamic);
	CuAssertPtrEquals(tc, NULL ,node->dynamicWord);
	
	free(word);
	word=NULL;
	deleteDataNode(node);
	free(node);
	node=NULL;
	
	node = malloc(sizeof(dataNode));
	node->staticArray = NULL;
	node->staticArrayLength = 0;
	
	word = malloc (sizeof(char) *(strlen("heterotransplantation")+1));
	strcpy(word,"heterotransplantation");
	
	insertString(node, word);
	CuAssertStrEquals(tc,word,node->dynamicWord);
	CuAssertIntEquals(tc, strlen(word) +1,node->noOfChars);
	CuAssertTrue(tc,node->isDynamic);
	CuAssertStrEquals(tc,"\0",node->word);
	
	free(word);
	word=NULL;
	deleteDataNode(node);
	free(node);
	node=NULL;
	
	
}


void TestGetString (CuTest *tc){
	dataNode* node = malloc(sizeof(dataNode));
	strcpy(node->word, "hello");
	node->isDynamic = false;
	node->dynamicWord=NULL;
	node->noOfChars = strlen("hello")+1;
	node->staticArray = NULL;
	node->staticArrayLength = 0;
	
	char* myStr = getString(node);
	
	CuAssertStrEquals(tc,"hello",myStr);

	node->dynamicWord = malloc(sizeof(char)* (strlen("heterotransplantation")+1));
	strcpy(node->dynamicWord, "heterotransplantation");
	node->noOfChars = strlen("heterotransplantation")+1;
	node->isDynamic = true;
	
	myStr = getString(node);
	
	CuAssertStrEquals(tc,"heterotransplantation",myStr);
	
	deleteDataNode(node);
	free(node);
	node=NULL; 
	
	
}

void TestCopyDataNode(CuTest* tc){
	dataNode* tempNode = malloc(sizeof(dataNode));
	initializeDataNode(tempNode);
	char* word = malloc (sizeof(char) *(strlen("hello")+1));
	strcpy(word,"hello");
	
	insertString(tempNode, word);
	tempNode->isFinal = true;
	
	dataNode* node = malloc(sizeof(dataNode));
	initializeDataNode(node);
	
	copyDataNode(node, tempNode);
	
	CuAssertStrEquals(tc,tempNode->word,node->word);
	CuAssertIntEquals(tc, tempNode->noOfChars,node->noOfChars);
	CuAssertTrue(tc,!node->isDynamic);
	CuAssertTrue(tc,node->isFinal);
	CuAssertIntEquals(tc,tempNode->staticArrayLength,node->staticArrayLength);
	CuAssertPtrEquals(tc,tempNode->staticArray,node->staticArray);
	CuAssertPtrEquals(tc,tempNode->nextWordArray,node->nextWordArray);
	CuAssertStrEquals(tc,tempNode->dynamicWord,node->dynamicWord);
	
	
	deleteDataNode(tempNode);
	free(tempNode);
	tempNode = NULL;
	deleteDataNode(node);
	free(node);
	node = NULL;
	free(word);
	word = NULL;
}

void TestBinarySearchBucket(CuTest* tc){

	
	arrayOfStructs* array_of_str = malloc(sizeof(arrayOfStructs));
	initializeArray(array_of_str);
	checkItemExists* getPosition;
	
	dataNode *item = malloc(sizeof(dataNode));
	initializeDataNode(item);
	getPosition = binarySearchBucket(array_of_str->array,item,0,array_of_str->position,array_of_str->position,NULL, true);			//array is empty
	CuAssertIntEquals(tc,-1,getPosition->position);
	CuAssertTrue(tc,!getPosition->exists);
	
	free(getPosition);
	getPosition = NULL;
	
		
	insertString(item,"cat");										//existing word
	insertString(&array_of_str->array[0],"cat");
	array_of_str->position++;
	insertString(&array_of_str->array[1],"dog");
	array_of_str->position++;
	insertString(&array_of_str->array[2],"whatabeautifuldaywego");
	array_of_str->position++;
	
	getPosition = binarySearchBucket(array_of_str->array,item,0,array_of_str->position,array_of_str->position,NULL, true);		//binarySeach cat
	CuAssertIntEquals(tc,0,getPosition->position);
	CuAssertTrue(tc,getPosition->exists);
	
	free(getPosition);
	getPosition = NULL;	
	
	deleteDataNode(item);
	free(item);
	item = NULL;
	
	item = malloc(sizeof(dataNode));
	initializeDataNode(item);
	insertString(item,"whatabeautifuldaywego");
	getPosition = binarySearchBucket(array_of_str->array,item,0,array_of_str->position,array_of_str->position,NULL, false);	
	CuAssertIntEquals(tc,2,getPosition->position);
	CuAssertTrue(tc,getPosition->exists);
		
	free(getPosition);
	getPosition = NULL;	
	
	deleteDataNode(item);
	free(item);
	item = NULL;
	
	item = malloc(sizeof(dataNode));
	initializeDataNode(item);
	insertString(item,"hello");	
	getPosition = binarySearchBucket(array_of_str->array,item,0,array_of_str->position,array_of_str->position,NULL, false);	
	CuAssertIntEquals(tc,2,getPosition->position);
	CuAssertTrue(tc,!getPosition->exists);
	
	free(getPosition);
	getPosition = NULL;
	
	deleteDataNode(item);
	free(item);
	item = NULL;
	deleteArray(array_of_str);
	array_of_str = NULL;

}

void TestInsertionSortBucket(CuTest* tc){

	HashTable* hashTable = createLinearHash(NOOFBUCKETS, NOOFCELLS);
	int lengthBefore = hashTable->buckets[0].length;

	dataNode* item = malloc(sizeof(dataNode));
	item->isFinal = true;
	initializeDataNode(item);

	insertString(item,"cat");

	hashTable->buckets[0].cells[0]= *item;
	hashTable->buckets[0].position++;
	
	

	insertString(item,"dog");    
	hashTable->buckets[0].cells[1]= *item;
	hashTable->buckets[0].position++;
	
	
	checkItemExists* getPosition = insertionSortBucket(hashTable,&hashTable->buckets[0],item,hashTable->buckets[0].position);		//try to insert dog
	CuAssertTrue(tc,getPosition->position==1);
	CuAssertTrue(tc,getPosition->exists);
	
	free(getPosition);
	getPosition = NULL;
	
	
	insertString(item,"hello"); 
	
	getPosition = insertionSortBucket(hashTable,&hashTable->buckets[0],item,hashTable->buckets[0].position);		
	hashTable->buckets[0].position++;               //non existing word
	CuAssertIntEquals(tc,2,getPosition->position);
	CuAssertTrue(tc,!getPosition->exists);
	
	free(getPosition);
	getPosition = NULL;
	
	
	insertString(item,"antidisestablishmentarianism");      
	
	getPosition = insertionSortBucket(hashTable,&hashTable->buckets[0],item,hashTable->buckets[0].position);		
	hashTable->buckets[0].position++;               //non existing word

	CuAssertIntEquals(tc,0,getPosition->position);
	CuAssertTrue(tc,!getPosition->exists);
	CuAssertStrEquals(tc,"antidisestablishmentarianism",hashTable->buckets[0].cells[0].dynamicWord);
	CuAssertStrEquals(tc,"cat",hashTable->buckets[0].cells[1].word);
	CuAssertStrEquals(tc,"dog",hashTable->buckets[0].cells[2].word);
	CuAssertStrEquals(tc,"hello",hashTable->buckets[0].cells[3].word);
	
	free(getPosition);
	getPosition = NULL;
	
	
	//check if bucket has been doubled
	
	CuAssertIntEquals(tc,2*lengthBefore,hashTable->buckets[0].length);
	
	free(item);
	item=NULL;
	
	destroyLinearHash(hashTable);
}


void TestHasChildren(CuTest* tc){
	dataNode* lookupElement = malloc(sizeof(dataNode));
	initializeDataNode(lookupElement);
	CuAssertTrue(tc,!hasChildren(lookupElement, -1));						//nextWordArray == NULL
	deleteDataNode(lookupElement);
	free(lookupElement);
	lookupElement = NULL;
	
	lookupElement = malloc(sizeof(dataNode));
	initializeDataNode(lookupElement);
	lookupElement->nextWordArray = malloc(sizeof(arrayOfStructs));
	initializeArray(lookupElement->nextWordArray);
	lookupElement->nextWordArray->position=1;
	CuAssertTrue(tc,hasChildren(lookupElement, 2));							//1 not deleted
	deleteArray(lookupElement->nextWordArray);
	deleteDataNode(lookupElement);
	free(lookupElement);
	lookupElement = NULL;
	
	lookupElement = malloc(sizeof(dataNode));
	initializeDataNode(lookupElement);
	lookupElement->nextWordArray = malloc(sizeof(arrayOfStructs));
	initializeArray(lookupElement->nextWordArray);
	lookupElement->nextWordArray->position=3;
	lookupElement->nextWordArray->array[0].deletionVersion = 2;
	lookupElement->nextWordArray->array[1].deletionVersion = 2;
	lookupElement->nextWordArray->array[2].deletionVersion = -1;
	CuAssertTrue(tc,hasChildren(lookupElement, 2));							//2 children deleted, 1 not deleted
	deleteArray(lookupElement->nextWordArray);
	deleteDataNode(lookupElement);
	free(lookupElement);
	lookupElement = NULL;
}

CuSuite* AuxMethodsGetSuite() {		//adding TestAuxMethods Functions into suite
    CuSuite* suite = CuSuiteNew();
    
    SUITE_ADD_TEST(suite, TestStringToArray);
    SUITE_ADD_TEST(suite, TestInitialize);
	SUITE_ADD_TEST(suite, TestBinarySearch);
	SUITE_ADD_TEST(suite, TestInsertionSort);
    //SUITE_ADD_TEST(suite, TestExecuteQueryFile_ExecuteDynamicArray);
    SUITE_ADD_TEST(suite, TestDeletionSort);
    SUITE_ADD_TEST(suite, TestGetString);
    SUITE_ADD_TEST(suite, TestInsertString);
    SUITE_ADD_TEST(suite, TestCopyDataNode);
    SUITE_ADD_TEST(suite, TestBinarySearchBucket);
    SUITE_ADD_TEST(suite, TestInsertionSortBucket);
    SUITE_ADD_TEST(suite, TestHasChildren);
    
    return suite;
}

