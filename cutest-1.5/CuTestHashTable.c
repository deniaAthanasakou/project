#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "CuTestHashTable.h"
#include "../auxMethods.h"

void TestCreateLinearHash(CuTest* tc){
	HashTable* hashTable = createLinearHash(NOOFBUCKETS, NOOFCELLS);
	
	CuAssertPtrNotNull(tc, hashTable);
	CuAssertIntEquals(tc, NOOFBUCKETS, hashTable->length);
	CuAssertIntEquals(tc, NOOFBUCKETS, hashTable->initialLength);
	CuAssertIntEquals(tc, NOOFBUCKETS, hashTable->numberOfBuckets);
	CuAssertIntEquals(tc, 0, hashTable->level);
	CuAssertIntEquals(tc, 0, hashTable->bucketToBeSplit);
	CuAssertPtrNotNull(tc, hashTable->buckets);
	
	destroyLinearHash(hashTable);
}


void TestDestroyLinearHash(CuTest* tc){
	HashTable* hashTable = createLinearHash(NOOFBUCKETS, NOOFCELLS);
	
	destroyLinearHash(hashTable);			//destroy empty hashTable
	hashTable = NULL;
	CuAssertPtrEquals(tc, NULL, hashTable);	
	
	hashTable = createLinearHash(NOOFBUCKETS, NOOFCELLS);
	dataNode* node = malloc(sizeof(dataNode));
	initializeDataNode(node);
	char* word = malloc(sizeof(char)*(strlen("python")+1));
	strcpy(word,"python");
	insertString(node,word);
	insertTrieNode(node,hashTable);
	free(word);
	word = NULL;
	
	word = malloc(sizeof(char)*(strlen("anaconda")+1));
	strcpy(word,"anaconda");
	insertString(node,word);
	insertTrieNode(node,hashTable);
	free(word);
	word = NULL;
	
	free(node);
	node = NULL;
	
	destroyLinearHash(hashTable);			//destroy non empty hashTable
	hashTable = NULL;
	CuAssertPtrEquals(tc, NULL, hashTable);	
	
}

void TestInsertTrieNode_InsertTrieNodeAgain_SplitBucket(CuTest* tc){
	HashTable* hashTable = createLinearHash(NOOFBUCKETS, NOOFCELLS);
	
	dataNode* node = malloc(sizeof(dataNode));
	initializeDataNode(node);
	char* word = malloc(sizeof(char)*(strlen("this")+1));
	strcpy(word,"this");
	insertString(node,word);
	dataNode* insertedNode = insertTrieNode(node,hashTable);
	free(word);
	word = NULL;
	
	CuAssertStrEquals(tc, "this", hashTable->buckets[0].cells[0].word);
	CuAssertIntEquals(tc, 1, hashTable->buckets[0].position);
	CuAssertPtrEquals(tc, insertedNode, &hashTable->buckets[0].cells[0]);
	
	word = malloc(sizeof(char)*(strlen("hits")+1));
	strcpy(word, "hits");
	insertString(node,word);
	insertedNode = insertTrieNode(node,hashTable);
	free(word);
	word = NULL;
	
	CuAssertStrEquals(tc, "hits", hashTable->buckets[0].cells[0].word);
	CuAssertStrEquals(tc, "this", hashTable->buckets[0].cells[1].word);
	CuAssertIntEquals(tc, 2, hashTable->buckets[0].position);
	CuAssertPtrEquals(tc, insertedNode, &hashTable->buckets[0].cells[0]);
	
	word = malloc(sizeof(char)*(strlen("hello")+1));
	strcpy(word,"hello");
	insertString(node,word);
	insertedNode = insertTrieNode(node,hashTable);
	free(word);
	word = NULL;
	
	CuAssertStrEquals(tc, "hello", hashTable->buckets[0].cells[0].word);
	CuAssertStrEquals(tc, "hits", hashTable->buckets[0].cells[1].word);
	CuAssertStrEquals(tc, "this", hashTable->buckets[0].cells[2].word);
	CuAssertIntEquals(tc, 3, hashTable->buckets[0].position);
	CuAssertPtrEquals(tc, insertedNode, &hashTable->buckets[0].cells[0]);
	
	word = malloc(sizeof(char)*(strlen("hits")+1));
	strcpy(word,"hits");
	insertString(node,word);
	insertedNode = insertTrieNode(node,hashTable);							//insert sth that already exists
	free(word);
	word = NULL;
	
	CuAssertStrEquals(tc, "hello", hashTable->buckets[0].cells[0].word);
	CuAssertStrEquals(tc, "hits", hashTable->buckets[0].cells[1].word);
	CuAssertStrEquals(tc, "this", hashTable->buckets[0].cells[2].word);
	CuAssertIntEquals(tc, 3, hashTable->buckets[0].position);
	CuAssertPtrEquals(tc, insertedNode, &hashTable->buckets[0].cells[1]);
	
	word = malloc(sizeof(char)*(strlen("house")+1));
	strcpy(word,"house");
	insertString(node,word);
	insertedNode = insertTrieNode(node,hashTable);
	free(word);
	word = NULL;
	
	CuAssertStrEquals(tc, "hello", hashTable->buckets[0].cells[0].word);
	CuAssertStrEquals(tc, "hits", hashTable->buckets[0].cells[1].word);
	CuAssertStrEquals(tc, "house", hashTable->buckets[0].cells[2].word);
	CuAssertStrEquals(tc, "this", hashTable->buckets[0].cells[3].word);
	CuAssertIntEquals(tc, 4, hashTable->buckets[0].position);
	CuAssertPtrEquals(tc, insertedNode, &hashTable->buckets[0].cells[2]);
	
	word = malloc(sizeof(char)*(strlen("tac")+1));
	strcpy(word,"tac");
	insertString(node,word);								//bucket will split
	insertedNode = insertTrieNode(node,hashTable);
	free(word);
	word = NULL;
	
	CuAssertStrEquals(tc, "hello", hashTable->buckets[0].cells[0].word);
	CuAssertStrEquals(tc, "hits", hashTable->buckets[0].cells[1].word);
	CuAssertStrEquals(tc, "house", hashTable->buckets[0].cells[2].word);
	CuAssertIntEquals(tc, 3, hashTable->buckets[0].position);
	
	CuAssertStrEquals(tc, "tac", hashTable->buckets[4].cells[0].word);
	CuAssertStrEquals(tc, "this", hashTable->buckets[4].cells[1].word);
	CuAssertIntEquals(tc, 2, hashTable->buckets[4].position);
	CuAssertIntEquals(tc, 4, hashTable->length);
	CuAssertIntEquals(tc, 5, hashTable->numberOfBuckets);
	CuAssertPtrEquals(tc, insertedNode, &hashTable->buckets[4].cells[0]);
	
	deleteDataNode(node);
	free(node);
	node = NULL;
	destroyLinearHash(hashTable);			
}


void TestInitializeBucket(CuTest* tc){
	Bucket* bucket = malloc(sizeof(Bucket)); 
	initializeBucket(bucket, NOOFCELLS);
	
	CuAssertIntEquals(tc, NOOFCELLS, bucket->length);
	CuAssertIntEquals(tc, 0, bucket->position);
	CuAssertIntEquals(tc, NOOFCELLS, bucket->noOfCells);
	CuAssertTrue(tc, !bucket->overflowed);
	CuAssertPtrNotNull(tc, bucket->cells);

	free(bucket->cells);
	bucket->cells=NULL;
	free(bucket);
	bucket = NULL;

}
void TestGetBucketFromHash(CuTest* tc){
	char* word = malloc((strlen("cat")+1)*sizeof(char));
	strcpy(word,"cat");
	int hash = getBucketFromHash(0, NOOFBUCKETS, 0, word, 3);
	free(word);
	word = NULL;
	CuAssertIntEquals(tc, 3, hash);
	
	word = malloc((strlen("hercules")+1)*sizeof(char));
	strcpy(word,"hercules");
	hash = getBucketFromHash(0, NOOFBUCKETS, 2, word, 8);
	free(word);
	word = NULL;
	CuAssertIntEquals(tc, 0, hash);
	
	word = malloc((strlen("cat")+1)*sizeof(char));
	strcpy(word,"cat");
	hash = getBucketFromHash(1, NOOFBUCKETS, 1, word, 3);
	free(word);
	word = NULL;
	CuAssertIntEquals(tc, 3, hash);
}



void TestLevelUp(CuTest* tc){
	HashTable* hashTable = createLinearHash(NOOFBUCKETS, NOOFCELLS);
	
	hashTable->bucketToBeSplit = 4;
	int oldBucket = levelUp(hashTable);			//must not level up
	
	CuAssertIntEquals(tc, -1, oldBucket);
	CuAssertIntEquals(tc, 0, hashTable->level);
	CuAssertIntEquals(tc, 4, hashTable->length);
	CuAssertIntEquals(tc, 4, hashTable->bucketToBeSplit);
	
	hashTable->numberOfBuckets = 8;
	oldBucket = levelUp(hashTable);			//must level up
	
	CuAssertIntEquals(tc, 3, oldBucket);
	CuAssertIntEquals(tc, 1, hashTable->level);
	CuAssertIntEquals(tc, 8, hashTable->length);
	CuAssertIntEquals(tc, 0, hashTable->bucketToBeSplit);
	
	hashTable->buckets = realloc(hashTable->buckets, 8 * sizeof(Bucket));		//for destroyLinearHash
	for(int i = 4; i<8; i++){
		initializeBucket(&hashTable->buckets[i], NOOFCELLS);
	}
	
	destroyLinearHash(hashTable);
}


void TestDeletionSortBucket(CuTest* tc){
	
	arrayOfStructs* array_of_str = NULL;
	HashTable* hashTable = createLinearHash(NOOFBUCKETS, NOOFCELLS);
	
	char** item = malloc(1*sizeof(char*));
	item[0] = malloc((strlen("hello")+1) * sizeof(char));
	strcpy(item[0],"hello");																
	insert_ngram(hashTable, item, 1, true);							//add 'hello'
	
	array_of_str = hashTable->buckets[0].cells[0].nextWordArray;
	deletionSortBucket(&hashTable->buckets[0],0);		//delete only element

	CuAssertIntEquals(tc,4,hashTable->buckets[0].length);	
	CuAssertIntEquals(tc,0,hashTable->buckets[0].position);	
	
	for(int i=0; i<1; i++){
		free(item[i]);
		item[i] = NULL;
	}
	free(item);
	item=NULL;
	
	item = malloc(1*sizeof(char*));
	item[0] = malloc((strlen("hello")+1) * sizeof(char));
	strcpy(item[0],"hello");																
	insert_ngram(hashTable, item, 1, 0);							//add 'hello'
	
	for(int i=0; i<1; i++){
		free(item[i]);
		item[i] = NULL;
	}
	free(item);
	item=NULL;													
	
	item = malloc(1*sizeof(char*));
	item[0] = malloc((strlen("heterotransplantations")+1) * sizeof(char));
	strcpy(item[0],"heterotransplantations");																
	insert_ngram(hashTable, item, 1, 1);							//add 'heterotransplantations'
	
	for(int i=0; i<1; i++){
		free(item[i]);
		item[i] = NULL;
	}
	free(item);
	item=NULL;		
	
	item = malloc(1*sizeof(char*));
	item[0] = malloc((strlen("hits")+1) * sizeof(char));
	strcpy(item[0],"hits");																
	insert_ngram(hashTable, item, 1, 2);							//add 'hits'
	
	for(int i=0; i<1; i++){
		free(item[i]);
		item[i] = NULL;
	}
	free(item);
	item=NULL;		
	
	item = malloc(2*sizeof(char*));
	item[0] = malloc((strlen("this")+1) * sizeof(char));
	strcpy(item[0],"this");	
	item[1] = malloc((strlen("is")+1) * sizeof(char));
	strcpy(item[1],"is");																
	insert_ngram(hashTable, item, 2, 3);								//add 'this is'
	
	for(int i=0; i<2; i++){
		free(item[i]);
		item[i] = NULL;
	}
	free(item);
	item=NULL;
	
	arrayOfStructs* arrayWillBeDeleted = hashTable->buckets[0].cells[3].nextWordArray;	
	
	array_of_str = hashTable->buckets[0].cells[0].nextWordArray;
	deletionSortBucket(&hashTable->buckets[0],3);		//from hello, heterotransplantations, hits, this delete this is
	arrayWillBeDeleted = NULL;
	CuAssertIntEquals(tc,4,hashTable->buckets[0].length);	
	CuAssertIntEquals(tc,3,hashTable->buckets[0].position);	
	
	CuAssertStrEquals(tc,"hello",hashTable->buckets[0].cells[0].word);
	CuAssertPtrEquals(tc,NULL,arrayWillBeDeleted);	 
	
	CuAssertStrEquals(tc,"heterotransplantations",hashTable->buckets[0].cells[1].dynamicWord); 
	CuAssertTrue(tc, hashTable->buckets[0].cells[1].isDynamic);
	CuAssertStrEquals(tc,"\0",hashTable->buckets[0].cells[1].word); 
	
	CuAssertStrEquals(tc,"hits",hashTable->buckets[0].cells[2].word); 
	
	item = malloc(1*sizeof(char*));
	item[0] = malloc((strlen("this")+1) * sizeof(char));
	strcpy(item[0],"this");																	
	insert_ngram(hashTable, item, 1, 4);								//add 'this'
	
	for(int i=0; i<1; i++){
		free(item[i]);
		item[i] = NULL;
	}
	free(item);
	item=NULL;	
	
	
	array_of_str = hashTable->buckets[0].cells[0].nextWordArray;
	deletionSortBucket(&hashTable->buckets[0],1);			//from hello, heterotransplantations, hits, this delete heterotransplantations
	CuAssertIntEquals(tc,4,hashTable->buckets[0].length);	
	CuAssertIntEquals(tc,3,hashTable->buckets[0].position);	
	
	CuAssertStrEquals(tc,"hello",hashTable->buckets[0].cells[0].word); 
	CuAssertStrEquals(tc,"hits",hashTable->buckets[0].cells[1].word); 
	CuAssertStrEquals(tc,"this",hashTable->buckets[0].cells[2].word); 
	

	destroyLinearHash(hashTable);
	hashTable=NULL;
	CuAssertPtrEquals(tc,NULL,hashTable);
}


void TestDeletionSortWithoutErasingNextArray(CuTest* tc){
	arrayOfStructs* array_of_str = NULL;
	HashTable* hashTable = createLinearHash(NOOFBUCKETS, NOOFCELLS);
	
	char** item = malloc(1*sizeof(char*));
	item[0] = malloc((strlen("hello")+1) * sizeof(char));
	strcpy(item[0],"hello");																
	insert_ngram(hashTable, item, 1, 0);							//add 'hello'
	
	array_of_str = hashTable->buckets[0].cells[0].nextWordArray;
	deletionSortWithoutErasingNextArray(&hashTable->buckets[0],0);		//delete only element

	CuAssertIntEquals(tc,4,hashTable->buckets[0].length);	
	CuAssertIntEquals(tc,0,hashTable->buckets[0].position);	
	
	
	for(int i=0; i<1; i++){
		free(item[i]);
		item[i] = NULL;
	}
	free(item);
	item=NULL;
	
	item = malloc(1*sizeof(char*));
	item[0] = malloc((strlen("hello")+1) * sizeof(char));
	strcpy(item[0],"hello");																
	insert_ngram(hashTable, item, 1, 1);							//add 'hello'
	
	
	for(int i=0; i<1; i++){
		free(item[i]);
		item[i] = NULL;
	}
	free(item);
	item=NULL;													
	
	item = malloc(1*sizeof(char*));
	item[0] = malloc((strlen("heterotransplantations")+1) * sizeof(char));
	strcpy(item[0],"heterotransplantations");																
	insert_ngram(hashTable, item, 1, 2);							//add 'heterotransplantations'
	
	
	for(int i=0; i<1; i++){
		free(item[i]);
		item[i] = NULL;
	}
	free(item);
	item=NULL;		
	
	item = malloc(1*sizeof(char*));
	item[0] = malloc((strlen("hits")+1) * sizeof(char));
	strcpy(item[0],"hits");																
	insert_ngram(hashTable, item, 1, 3);							//add 'hits'
	
	
	for(int i=0; i<1; i++){
		free(item[i]);
		item[i] = NULL;
	}
	free(item);
	item=NULL;		
	
	item = malloc(2*sizeof(char*));
	item[0] = malloc((strlen("this")+1) * sizeof(char));
	strcpy(item[0],"this");	
	item[1] = malloc((strlen("is")+1) * sizeof(char));
	strcpy(item[1],"is");																
	insert_ngram(hashTable, item, 2, 4);								//add 'this is'
	
	
	for(int i=0; i<2; i++){
		free(item[i]);
		item[i] = NULL;
	}
	free(item);
	item=NULL;
	
	arrayOfStructs* arrayWillNotBeDeleted = hashTable->buckets[0].cells[3].nextWordArray;
	array_of_str = hashTable->buckets[0].cells[0].nextWordArray;
	deletionSortWithoutErasingNextArray(&hashTable->buckets[0],3);		//from hello, heterotransplantations, hits, this delete this is
	
	CuAssertIntEquals(tc,4,hashTable->buckets[0].length);	
	CuAssertIntEquals(tc,3,hashTable->buckets[0].position);	
	
	CuAssertStrEquals(tc,"hello",hashTable->buckets[0].cells[0].word); 
	
	CuAssertStrEquals(tc,"heterotransplantations",hashTable->buckets[0].cells[1].dynamicWord); 
	CuAssertTrue(tc, hashTable->buckets[0].cells[1].isDynamic);
	CuAssertStrEquals(tc,"\0",hashTable->buckets[0].cells[1].word); 
	
	CuAssertStrEquals(tc,"hits",hashTable->buckets[0].cells[2].word); 
	CuAssertPtrNotNull(tc, arrayWillNotBeDeleted);
	
	
	item = malloc(1*sizeof(char*));
	item[0] = malloc((strlen("this")+1) * sizeof(char));
	strcpy(item[0],"this");																	
	insert_ngram(hashTable, item, 1, 5);								//add 'this'
	
	
	for(int i=0; i<1; i++){
		free(item[i]);
		item[i] = NULL;
	}
	free(item);
	item=NULL;	
	
	
	array_of_str = hashTable->buckets[0].cells[0].nextWordArray;
	deletionSortWithoutErasingNextArray(&hashTable->buckets[0],1);			//from hello, heterotransplantations, hits, this delete heterotransplantations
	CuAssertIntEquals(tc,4,hashTable->buckets[0].length);	
	CuAssertIntEquals(tc,3,hashTable->buckets[0].position);	
	
	CuAssertStrEquals(tc,"hello",hashTable->buckets[0].cells[0].word); 
	CuAssertStrEquals(tc,"hits",hashTable->buckets[0].cells[1].word); 
	CuAssertStrEquals(tc,"this",hashTable->buckets[0].cells[2].word); 
	
	deleteArray(arrayWillNotBeDeleted);

	destroyLinearHash(hashTable);
	hashTable=NULL;
	CuAssertPtrEquals(tc,NULL,hashTable);
}

void TestCreateOverflowCells(CuTest* tc){
	Bucket* bucket = malloc(sizeof(Bucket)); 
	initializeBucket(bucket, NOOFCELLS);
	int oldLength = bucket->length;
	createOverflowCells(bucket);
	
	CuAssertIntEquals(tc, 8, bucket->length);
	CuAssertIntEquals(tc, NOOFCELLS, bucket->noOfCells);
	CuAssertTrue(tc, bucket->overflowed);
	CuAssertPtrNotNull(tc, bucket->cells);

	free(bucket->cells);
	bucket->cells=NULL;
	free(bucket);
	bucket = NULL;
}

void TestGetCell(CuTest* tc){
	HashTable* hashTable = createLinearHash(NOOFBUCKETS, NOOFCELLS);
	
	dataNode* node = malloc(sizeof(dataNode));
	initializeDataNode(node);
	char* word = malloc(sizeof(char)*(strlen("this")+1));
	strcpy(word,"this");
	insertString(node,word);
	insertTrieNode(node,hashTable);
	free(word);
	word = NULL;
	
	CuAssertStrEquals(tc, "this", hashTable->buckets[0].cells[0].word);
	CuAssertIntEquals(tc, 1, hashTable->buckets[0].position);
	
	word = malloc(sizeof(char)*(strlen("hits")+1));
	strcpy(word, "hits");
	insertString(node,word);
	insertTrieNode(node,hashTable);
	free(word);
	word = NULL;
	
	CuAssertStrEquals(tc, "hits", hashTable->buckets[0].cells[0].word);
	CuAssertStrEquals(tc, "this", hashTable->buckets[0].cells[1].word);
	CuAssertIntEquals(tc, 2, hashTable->buckets[0].position);
	
	word = malloc(sizeof(char)*(strlen("hello")+1));
	strcpy(word,"hello");
	insertString(node,word);
	insertTrieNode(node,hashTable);
	free(word);
	word = NULL;
	
	CuAssertStrEquals(tc, "hello", hashTable->buckets[0].cells[0].word);
	CuAssertStrEquals(tc, "hits", hashTable->buckets[0].cells[1].word);
	CuAssertStrEquals(tc, "this", hashTable->buckets[0].cells[2].word);
	CuAssertIntEquals(tc, 3, hashTable->buckets[0].position);
	
	word = malloc(sizeof(char)*(strlen("hits")+1));
	strcpy(word,"hits");
	insertString(node,word);
	insertTrieNode(node,hashTable);							//insert sth that already exists
	free(word);
	word = NULL;
	
	CuAssertStrEquals(tc, "hello", hashTable->buckets[0].cells[0].word);
	CuAssertStrEquals(tc, "hits", hashTable->buckets[0].cells[1].word);
	CuAssertStrEquals(tc, "this", hashTable->buckets[0].cells[2].word);
	CuAssertIntEquals(tc, 3, hashTable->buckets[0].position);
	
	word = malloc(sizeof(char)*(strlen("house")+1));
	strcpy(word,"house");
	insertString(node,word);
	insertTrieNode(node,hashTable);
	free(word);
	word = NULL;
	
	CuAssertStrEquals(tc, "hello", hashTable->buckets[0].cells[0].word);
	CuAssertStrEquals(tc, "hits", hashTable->buckets[0].cells[1].word);
	CuAssertStrEquals(tc, "house", hashTable->buckets[0].cells[2].word);
	CuAssertStrEquals(tc, "this", hashTable->buckets[0].cells[3].word);
	CuAssertIntEquals(tc, 4, hashTable->buckets[0].position);
	
	word = malloc(sizeof(char)*(strlen("tac")+1));
	strcpy(word,"tac");
	insertString(node,word);								//bucket will split
	insertTrieNode(node,hashTable);
	free(word);
	word = NULL;
	
	CuAssertStrEquals(tc, "hello", hashTable->buckets[0].cells[0].word);
	CuAssertStrEquals(tc, "hits", hashTable->buckets[0].cells[1].word);
	CuAssertStrEquals(tc, "house", hashTable->buckets[0].cells[2].word);
	CuAssertIntEquals(tc, 3, hashTable->buckets[0].position);
	
	CuAssertStrEquals(tc, "tac", hashTable->buckets[4].cells[0].word);
	CuAssertStrEquals(tc, "this", hashTable->buckets[4].cells[1].word);
	CuAssertIntEquals(tc, 2, hashTable->buckets[4].position);
	CuAssertIntEquals(tc, 4, hashTable->length);
	CuAssertIntEquals(tc, 5, hashTable->numberOfBuckets);
	
	deleteDataNode(node);
	free(node);
	node = NULL;
	
	
	word = malloc(sizeof(char)*(strlen("tac")+1));
	strcpy(word,"tac");
	int cell = getCell(word , hashTable,4);		//word that exists
	CuAssertIntEquals(tc, 0, cell);
	free(word);
	word = NULL;
	
	word = malloc(sizeof(char)*(strlen("coconut")+1));
	strcpy(word,"coconut");							
	cell = getCell(word , hashTable,0);		//word that doesn't exist
	CuAssertIntEquals(tc, -1, cell);
	free(word);
	word = NULL;
	
	
	destroyLinearHash(hashTable);
}



void TestLookupTrieNode(CuTest* tc){
	HashTable* hashTable = createLinearHash(NOOFBUCKETS, NOOFCELLS);
	dataNode* node = malloc(sizeof(dataNode));
	initializeDataNode(node);
	char* word = malloc(sizeof(char)*(strlen("this")+1));
	strcpy(word,"this");
	insertString(node,word);
	insertTrieNode(node,hashTable);
	free(word);
	word = NULL;
	
	CuAssertStrEquals(tc, "this", hashTable->buckets[0].cells[0].word);
	CuAssertIntEquals(tc, 1, hashTable->buckets[0].position);
	
	word = malloc(sizeof(char)*(strlen("hits")+1));
	strcpy(word, "hits");
	insertString(node,word);
	insertTrieNode(node,hashTable);
	free(word);
	word = NULL;
	
	CuAssertStrEquals(tc, "hits", hashTable->buckets[0].cells[0].word);
	CuAssertStrEquals(tc, "this", hashTable->buckets[0].cells[1].word);
	CuAssertIntEquals(tc, 2, hashTable->buckets[0].position);
	
	word = malloc(sizeof(char)*(strlen("hello")+1));
	strcpy(word,"hello");
	insertString(node,word);
	insertTrieNode(node,hashTable);
	free(word);
	word = NULL;
	
	CuAssertStrEquals(tc, "hello", hashTable->buckets[0].cells[0].word);
	CuAssertStrEquals(tc, "hits", hashTable->buckets[0].cells[1].word);
	CuAssertStrEquals(tc, "this", hashTable->buckets[0].cells[2].word);
	CuAssertIntEquals(tc, 3, hashTable->buckets[0].position);
	
	word = malloc(sizeof(char)*(strlen("hits")+1));
	strcpy(word,"hits");
	insertString(node,word);
	insertTrieNode(node,hashTable);							//insert sth that already exists
	free(word);
	word = NULL;
	
	CuAssertStrEquals(tc, "hello", hashTable->buckets[0].cells[0].word);
	CuAssertStrEquals(tc, "hits", hashTable->buckets[0].cells[1].word);
	CuAssertStrEquals(tc, "this", hashTable->buckets[0].cells[2].word);
	CuAssertIntEquals(tc, 3, hashTable->buckets[0].position);
	
	word = malloc(sizeof(char)*(strlen("house")+1));
	strcpy(word,"house");
	insertString(node,word);
	insertTrieNode(node,hashTable);
	free(word);
	word = NULL;
	
	CuAssertStrEquals(tc, "hello", hashTable->buckets[0].cells[0].word);
	CuAssertStrEquals(tc, "hits", hashTable->buckets[0].cells[1].word);
	CuAssertStrEquals(tc, "house", hashTable->buckets[0].cells[2].word);
	CuAssertStrEquals(tc, "this", hashTable->buckets[0].cells[3].word);
	CuAssertIntEquals(tc, 4, hashTable->buckets[0].position);
	
	word = malloc(sizeof(char)*(strlen("tac")+1));
	strcpy(word,"tac");
	insertString(node,word);								//bucket will split
	insertTrieNode(node,hashTable);
	free(word);
	word = NULL;
	
	CuAssertStrEquals(tc, "hello", hashTable->buckets[0].cells[0].word);
	CuAssertStrEquals(tc, "hits", hashTable->buckets[0].cells[1].word);
	CuAssertStrEquals(tc, "house", hashTable->buckets[0].cells[2].word);
	CuAssertIntEquals(tc, 3, hashTable->buckets[0].position);
	
	CuAssertStrEquals(tc, "tac", hashTable->buckets[4].cells[0].word);
	CuAssertStrEquals(tc, "this", hashTable->buckets[4].cells[1].word);
	CuAssertIntEquals(tc, 2, hashTable->buckets[4].position);
	CuAssertIntEquals(tc, 4, hashTable->length);
	CuAssertIntEquals(tc, 5, hashTable->numberOfBuckets);
	
	deleteDataNode(node);
	free(node);
	node = NULL;
	
	
	word = malloc(sizeof(char)*(strlen("tac")+1));
	strcpy(word,"tac");
	dataNode* lookUpNode = lookupTrieNode(word , hashTable);		//word that exists
	CuAssertPtrEquals(tc, &hashTable->buckets[4].cells[0], lookUpNode);
	free(word);
	word = NULL;
	
	word = malloc(sizeof(char)*(strlen("coconut")+1));
	strcpy(word,"coconut");							
	lookUpNode = lookupTrieNode(word , hashTable);		//word that doesn't exist
	CuAssertPtrEquals(tc, NULL, lookUpNode);
	free(word);
	word = NULL;
	
	destroyLinearHash(hashTable);
}

void TestRestructHashTable(CuTest* tc){

	HashTable* hashTable = createLinearHash(NOOFBUCKETS,NOOFCELLS);
	
	char** item = malloc(5*sizeof(char*));
	item[0] = malloc((strlen("hello")+1) * sizeof(char));
	strcpy(item[0],"hello");	
	item[1] = malloc((strlen("hello1")+1) * sizeof(char));
	strcpy(item[1],"hello1");																			
	insert_ngram(hashTable, item, 2, 1);
	
	for(int i=0; i<2; i++){
		free(item[i]);
		item[i] = NULL;
	}	
	free(item);
	item = NULL;
	
	item = malloc(5*sizeof(char*));
	item[0] = malloc((strlen("cat")+1) * sizeof(char));
	strcpy(item[0],"cat");	
	item[1] = malloc((strlen("hello1")+1) * sizeof(char));
	strcpy(item[1],"hello1");																			
	insert_ngram(hashTable, item, 2, 1);
	
	for(int i=0; i<2; i++){
		free(item[i]);
		item[i] = NULL;
	}	
	free(item);
	item = NULL;
	
	hashTable->buckets[0].cells[0].nextWordArray->array[0].deletionVersion = -1;
	hashTable->buckets[3].cells[0].nextWordArray->array[0].deletionVersion = -1;
	
					
	restructHashTable(hashTable);			//nothing will be deleted
	
	CuAssertIntEquals(tc, 1, hashTable->buckets[0].cells[0].nextWordArray->position);
	CuAssertIntEquals(tc, 1, hashTable->buckets[3].cells[0].nextWordArray->position);
	
	hashTable->buckets[0].cells[0].nextWordArray->array[0].deletionVersion = -1;
	hashTable->buckets[3].cells[0].nextWordArray->array[0].deletionVersion = 1;
	
					
	restructHashTable(hashTable);			//cat will be deleted
	
	CuAssertIntEquals(tc, 1, hashTable->buckets[0].cells[0].nextWordArray->position);
	CuAssertIntEquals(tc, 0, hashTable->buckets[3].cells[0].nextWordArray->position);
	
	destroyLinearHash(hashTable);

}


void TestRestructArray(CuTest* tc){
	HashTable* hashTable = createLinearHash(NOOFBUCKETS,NOOFCELLS);
	
	char** item = malloc(5*sizeof(char*));
	item[0] = malloc((strlen("hello")+1) * sizeof(char));
	strcpy(item[0],"hello");	
	item[1] = malloc((strlen("hello1")+1) * sizeof(char));
	strcpy(item[1],"hello1");			
	item[2] = malloc((strlen("hello2")+1) * sizeof(char));
	strcpy(item[2],"hello2");			
	item[3] = malloc((strlen("hello3")+1) * sizeof(char));
	strcpy(item[3],"hello3");			
	item[4] = malloc((strlen("hello4")+1) * sizeof(char));
	strcpy(item[4],"hello4");																		
	insert_ngram(hashTable, item, 5, 1);
	
	for(int i=0; i<5; i++){
		free(item[i]);
		item[i] = NULL;
	}	
	free(item);
	item = NULL;					
	
	hashTable->buckets[0].cells[0].nextWordArray->array[0].deletionVersion = -1;
	restructArray(hashTable->buckets[0].cells[0].nextWordArray);
	CuAssertIntEquals(tc, 1, hashTable->buckets[0].cells[0].nextWordArray->position);
	
	hashTable->buckets[0].cells[0].nextWordArray->array[0].deletionVersion = 1;
	restructArray(hashTable->buckets[0].cells[0].nextWordArray);
	CuAssertIntEquals(tc, 0, hashTable->buckets[0].cells[0].nextWordArray->position);
	
	destroyLinearHash(hashTable);
	hashTable = createLinearHash(NOOFBUCKETS,NOOFCELLS);
	item = malloc(5*sizeof(char*));
	item[0] = malloc((strlen("hello")+1) * sizeof(char));
	strcpy(item[0],"hello");	
	item[1] = malloc((strlen("hello1")+1) * sizeof(char));
	strcpy(item[1],"hello1");			
	item[2] = malloc((strlen("hello2")+1) * sizeof(char));
	strcpy(item[2],"hello2");			
	item[3] = malloc((strlen("hello3")+1) * sizeof(char));
	strcpy(item[3],"hello3");			
	item[4] = malloc((strlen("hello4")+1) * sizeof(char));
	strcpy(item[4],"hello4");																		
	insert_ngram(hashTable, item, 5, 1);	
	
	for(int i=0; i<5; i++){
		free(item[i]);
		item[i] = NULL;
	}	
	free(item);
	item = NULL;
	
	item = malloc(5*sizeof(char*));
	item[0] = malloc((strlen("hello")+1) * sizeof(char));
	strcpy(item[0],"hello");	
	item[1] = malloc((strlen("this1")+1) * sizeof(char));
	strcpy(item[1],"this1");			
	item[2] = malloc((strlen("this2")+1) * sizeof(char));
	strcpy(item[2],"this2");			
	item[3] = malloc((strlen("this3")+1) * sizeof(char));
	strcpy(item[3],"this3");			
	item[4] = malloc((strlen("this4")+1) * sizeof(char));
	strcpy(item[4],"this4");
	
																							
	insert_ngram(hashTable, item, 5, 1);	
	
	for(int i=0; i<5; i++){
		free(item[i]);
		item[i] = NULL;
	}	
	free(item);
	item = NULL;					
	
	CuAssertIntEquals(tc, 2, hashTable->buckets[0].cells[0].nextWordArray->position);
	hashTable->buckets[0].cells[0].nextWordArray->array[0].deletionVersion = -1;
	hashTable->buckets[0].cells[0].nextWordArray->array[1].deletionVersion = 1;
	restructArray(hashTable->buckets[0].cells[0].nextWordArray);
	CuAssertIntEquals(tc, 1, hashTable->buckets[0].cells[0].nextWordArray->position);
	
	
	destroyLinearHash(hashTable);
	hashTable = createLinearHash(NOOFBUCKETS,NOOFCELLS);
	
		
	item = malloc(5*sizeof(char*));
	item[0] = malloc((strlen("hello")+1) * sizeof(char));
	strcpy(item[0],"hello");	
	item[1] = malloc((strlen("a1")+1) * sizeof(char));
	strcpy(item[1],"a1");			
	item[2] = malloc((strlen("a2")+1) * sizeof(char));
	strcpy(item[2],"a2");			
	item[3] = malloc((strlen("a3")+1) * sizeof(char));
	strcpy(item[3],"a3");			
	item[4] = malloc((strlen("a4")+1) * sizeof(char));
	strcpy(item[4],"a4");																		
	insert_ngram(hashTable, item, 5, 1);	
	
	for(int i=0; i<5; i++){
		free(item[i]);
		item[i] = NULL;
	}	
	free(item);
	item = NULL;
	
	item = malloc(5*sizeof(char*));
	item[0] = malloc((strlen("hello")+1) * sizeof(char));
	strcpy(item[0],"hello");	
	item[1] = malloc((strlen("hello1")+1) * sizeof(char));
	strcpy(item[1],"hello1");			
	item[2] = malloc((strlen("hello2")+1) * sizeof(char));
	strcpy(item[2],"hello2");			
	item[3] = malloc((strlen("hello3")+1) * sizeof(char));
	strcpy(item[3],"hello3");			
	item[4] = malloc((strlen("hello4")+1) * sizeof(char));
	strcpy(item[4],"hello4");																		
	insert_ngram(hashTable, item, 5, 1);	
	
	for(int i=0; i<5; i++){
		free(item[i]);
		item[i] = NULL;
	}	
	free(item);
	item = NULL;
	
	item = malloc(5*sizeof(char*));
	item[0] = malloc((strlen("hello")+1) * sizeof(char));
	strcpy(item[0],"hello");	
	item[1] = malloc((strlen("this1")+1) * sizeof(char));
	strcpy(item[1],"this1");			
	item[2] = malloc((strlen("this2")+1) * sizeof(char));
	strcpy(item[2],"this2");			
	item[3] = malloc((strlen("this3")+1) * sizeof(char));
	strcpy(item[3],"this3");			
	item[4] = malloc((strlen("this4")+1) * sizeof(char));
	strcpy(item[4],"this4");
	
																							
	insert_ngram(hashTable, item, 5, 1);	
	
	for(int i=0; i<5; i++){
		free(item[i]);
		item[i] = NULL;
	}	
	free(item);
	item = NULL;	
	
	item = malloc(5*sizeof(char*));
	item[0] = malloc((strlen("hello")+1) * sizeof(char));
	strcpy(item[0],"hello");	
	item[1] = malloc((strlen("is1")+1) * sizeof(char));
	strcpy(item[1],"is1");			
	item[2] = malloc((strlen("is2")+1) * sizeof(char));
	strcpy(item[2],"is2");			
	item[3] = malloc((strlen("is3")+1) * sizeof(char));
	strcpy(item[3],"is3");			
	item[4] = malloc((strlen("is4")+1) * sizeof(char));
	strcpy(item[4],"is4");
	
																							
	insert_ngram(hashTable, item, 5, 1);	
	
	for(int i=0; i<5; i++){
		free(item[i]);
		item[i] = NULL;
	}	
	free(item);
	item = NULL;									
	
	CuAssertIntEquals(tc, 4, hashTable->buckets[0].cells[0].nextWordArray->position);
	hashTable->buckets[0].cells[0].nextWordArray->array[0].deletionVersion = -1;
	hashTable->buckets[0].cells[0].nextWordArray->array[1].deletionVersion = 1;
	hashTable->buckets[0].cells[0].nextWordArray->array[2].deletionVersion = -1;
	hashTable->buckets[0].cells[0].nextWordArray->array[3].deletionVersion = 1;
	restructArray(hashTable->buckets[0].cells[0].nextWordArray);										//delete hello1 and this1
	CuAssertIntEquals(tc, 2, hashTable->buckets[0].cells[0].nextWordArray->position);
	
	destroyLinearHash(hashTable);
	
}

CuSuite* HashTableGetSuite() {		//adding TestHashTable Functions into suite
    CuSuite* suite = CuSuiteNew();
    
    SUITE_ADD_TEST(suite, TestCreateLinearHash);
    SUITE_ADD_TEST(suite, TestDestroyLinearHash);
    SUITE_ADD_TEST(suite, TestInsertTrieNode_InsertTrieNodeAgain_SplitBucket);
    SUITE_ADD_TEST(suite, TestInitializeBucket);
    SUITE_ADD_TEST(suite, TestGetBucketFromHash);
    SUITE_ADD_TEST(suite, TestLevelUp);
    SUITE_ADD_TEST(suite, TestCreateOverflowCells);
    SUITE_ADD_TEST(suite, TestGetCell);
    SUITE_ADD_TEST(suite, TestDeletionSortWithoutErasingNextArray);
    SUITE_ADD_TEST(suite, TestDeletionSortBucket);
    SUITE_ADD_TEST(suite, TestLookupTrieNode);
    SUITE_ADD_TEST(suite, TestRestructHashTable);
    SUITE_ADD_TEST(suite, TestRestructArray);
 
    
    return suite;
}
