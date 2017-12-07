#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "hashTable.h"
#include "auxMethods.h"

HashTable* createLinearHash(int noOfBuckets, int noOfCells){
	HashTable* hashTable = malloc(1*sizeof(HashTable));
	hashTable->buckets = malloc(noOfBuckets * sizeof(Bucket));
	hashTable->length = noOfBuckets;
	hashTable->initialLength = noOfBuckets;
	hashTable->numberOfBuckets = noOfBuckets;
	hashTable->level = 0;
	hashTable->bucketToBeSplit = 0;
	for(int i=0; i<noOfBuckets; i++){			//for each bucket
		initializeBucket(&(hashTable->buckets[i]),noOfCells);
	}
	return hashTable;
}

void initializeBucket(Bucket* bucket, int noOfCells){
	bucket->length = noOfCells;
	bucket->noOfCells = noOfCells;
	bucket->position = 0;
	bucket->cells = malloc(noOfCells * sizeof(dataNode));
	bucket->overflowed = false;
	for(int i=0; i<noOfCells; i++){
		initializeDataNode(&bucket->cells[i]);
	}
}

void deletionSortWithoutErasingNextArray(Bucket* bucket, int position){
	if(bucket->cells[position].dynamicWord!=NULL){	
		free(bucket->cells[position].dynamicWord);
		bucket->cells[position].dynamicWord=NULL;
	}
	else{
		bucket->cells[position].word[0] = '\0';
	}
	
	int fullMoveSize = 0;
	if(position < bucket->position -1){
		fullMoveSize = (bucket->position -1 - position)*sizeof(bucket->cells[position+1]);
	}
	bucket->cells[position] = bucket->cells[position+1];
	memmove(&(bucket->cells[position]),&(bucket->cells[position+1]), fullMoveSize);
	bucket->position--;
}


void deletionSortBucket(Bucket* bucket, int position){

	if(bucket->cells[position].dynamicWord!=NULL){	
		free(bucket->cells[position].dynamicWord);
		bucket->cells[position].dynamicWord=NULL;
	}
	else{
		bucket->cells[position].word[0] = '\0';
	}
	if(bucket->cells[position].nextWordArray!=NULL){
		deleteArray(bucket->cells[position].nextWordArray);
		bucket->cells[position].nextWordArray=NULL;
	}
	
	int moveSize = 0;
	int fullMoveSize = 0;
	int startingPoint = position;
	while (position < bucket->position -1)
	{
	    moveSize+=sizeof(bucket->cells[position+1]);
	    position++;
	}
	if(moveSize!=0)
		fullMoveSize = (bucket->position -1 - position)*sizeof(bucket->cells[position+1]);
		
	memmove(&(bucket->cells[startingPoint]),&(bucket->cells[startingPoint+1]), moveSize);
	bucket->position--;
}


dataNode* insertTrieNode(dataNode* node, HashTable* hashTable){
	

	char* word = getString(node);
	
	int noOfbucket = getBucketFromHash(hashTable->level, hashTable->initialLength, hashTable->bucketToBeSplit, word);	
	
	Bucket* bucket = &(hashTable->buckets[noOfbucket]);
	
	checkItemExists* check;	
	check = insertionSortBucket(hashTable, &(hashTable->buckets[noOfbucket]),node,bucket->position);
	
	dataNode* returnNodePtr = check->insertedNode;
	
	if(bucket->overflowed){
		bucket->overflowed = false;
		hashTable->bucketToBeSplit++;
		char *tempWord = getString(node);
		dataNode *tempNode =  splitBucket(hashTable,tempWord);
		if(tempNode!=NULL)
			returnNodePtr = tempNode;
		
		free(tempWord);
		tempWord = NULL;
	}
	
	free(word);
	word=NULL; 
	
	if(check!=NULL){
		free(check);
		check=NULL;
	}
	return returnNodePtr;
}



dataNode* insertTrieNodeAgain(dataNode* node, HashTable* hashTable, int checkBucketToBeSplit){	
	
	char* word = getString(node);
	int noOfbucket = getBucketFromHash(hashTable->level, hashTable->initialLength, hashTable->bucketToBeSplit, word);
	
	dataNode splitted;
	if(checkBucketToBeSplit!=-1){
		if(noOfbucket!=checkBucketToBeSplit){
			Bucket* temp = &(hashTable->buckets[checkBucketToBeSplit]);
			int prevPos = getCell(word,hashTable,checkBucketToBeSplit);
			splitted = hashTable->buckets[checkBucketToBeSplit].cells[prevPos];
			deletionSortWithoutErasingNextArray(temp,prevPos);
		}
	}
	Bucket* bucket = &(hashTable->buckets[noOfbucket]);
	
	//insertion sort
	checkItemExists* check;
	if(checkBucketToBeSplit==-1 || noOfbucket==checkBucketToBeSplit){
		check = insertionSortBucket(hashTable, &(hashTable->buckets[noOfbucket]),node,bucket->position);
	}
	else{
		if(noOfbucket!=checkBucketToBeSplit){
			check = insertionSortBucket(hashTable, &(hashTable->buckets[noOfbucket]),&splitted,bucket->position);
		}		
	}
	free(word);
	word=NULL;
	 dataNode* returnNodePtr = check->insertedNode;
	
	if(check!=NULL){
		free(check);
		check=NULL;
	}

	return returnNodePtr;
}



void createOverflowCells(Bucket* bucket){
	int oldLength = bucket->length;
	bucket->length += bucket->noOfCells;
	bucket->overflowed = true;
	bucket->cells = realloc(bucket->cells, bucket->length * sizeof(dataNode));
	for(int i=oldLength; i<bucket->length; i++){
		initializeDataNode(&bucket->cells[i]);
	}
}

void printBuckets(HashTable* hashTable){
	int j;
	printf("Printing buckets\n");
	for(j=0; j < hashTable->numberOfBuckets; j++){
		printf("----%dTH BUCKET----\n",j);
		printBucket(&(hashTable->buckets[j]));
	}
}

void printBucket(Bucket *bucket){
	for(int i=0; i < bucket->position;i++){
		char *word = getString(&bucket->cells[i]);
		printf("%dth elem with word: '%s'",i,word);
		if(bucket->cells[i].isFinal)
			printf(" FINAL\n ");
		else
			printf(" NOT FINAL\n");
		if(bucket->cells[i].nextWordArray!=NULL)
			printFullArray(bucket->cells[i].nextWordArray, bucket->cells[i].nextWordArray->position);
		free(word);
		word = NULL;
	}
}

int levelUp(HashTable* hashTable){
	if(hashTable->numberOfBuckets == 2*(hashTable->length)){
		hashTable->level++;
		hashTable->length = 2*(hashTable->length);
		int oldBucket = hashTable->bucketToBeSplit -1;
		hashTable->bucketToBeSplit=0; 	//re-initialize bucketToBeSplit
		return oldBucket;
	}
	return -1;	
		
}


dataNode* splitBucket(HashTable* hashTable, char* oldCellWord){
	hashTable->numberOfBuckets++;
	hashTable->buckets = realloc(hashTable->buckets, hashTable->numberOfBuckets * sizeof(Bucket));
	
	int nextCreatedBucket = hashTable->numberOfBuckets-1;
	initializeBucket(&hashTable->buckets[nextCreatedBucket], hashTable->buckets[0].noOfCells);
	int oldBucket = levelUp(hashTable);	
	
	if(oldBucket==-1){		//level wasn't increased
		oldBucket = hashTable->bucketToBeSplit -1;
	}	
	
	//for each dataNode in bucketToBeSplit call getHashBucket and rearrange
	Bucket *tempBucket = &(hashTable->buckets[oldBucket]);
	dataNode* returnNode = NULL;
	for(int i=0; i<tempBucket->position; i++){		//for each cell of bucket

		char* oldWord=  getString(&tempBucket->cells[i]);		
		dataNode* tempNode = insertTrieNodeAgain(&tempBucket->cells[i],hashTable,oldBucket);
		char* tempWord = getString(tempNode);
		if(strcmp(tempWord, oldCellWord)==0)
			returnNode=tempNode;
			
		free(tempWord);
		tempWord = NULL;	
		
		int newBucket = getBucketFromHash(hashTable->level,hashTable->initialLength, hashTable->bucketToBeSplit,oldWord);
		if(newBucket!=oldBucket){	//node was moved to another bucket
			i--;
		}
		
		if(oldWord!=NULL){
			free(oldWord);
			oldWord=NULL;
		}		
	}
	return returnNode;
}


dataNode* lookupTrieNode(char* lookupWord ,HashTable* hashTable){
	int getCorrectBucket = getBucketFromHash(hashTable->level, hashTable->initialLength, hashTable->bucketToBeSplit, lookupWord);
	Bucket *bucket = &hashTable->buckets[getCorrectBucket];
	int maxElems = bucket->position;
	
	int first = 0;
	int last = maxElems - 1;
	int middle = (first+last)/2;
	while (first <= last) {
		char *searchWord = NULL;
		searchWord = getString(&bucket->cells[middle]);
		if(bucket->cells[middle].staticArray!=NULL){
			int sizeMalloc = abs(bucket->cells[middle].staticArray[0]);
			//printf("sizeMalloc is LOOKUPTRIE'%d'\n",sizeMalloc);
			int j = 0;
			int k=0;
			for(j=0; j< sizeMalloc; j++){
				searchWord[j] = bucket->cells[middle].dynamicWord[k];
				k++;
			}
			searchWord[j] = '\0';
		}
		//printf("searchword %s\n",searchWord);
		if (strcmp(searchWord,lookupWord)<0)
			first = middle + 1;    
		else if (strcmp(searchWord,lookupWord)==0) {
			free(searchWord);
			searchWord = NULL;
			dataNode* returnNode =  &bucket->cells[middle];
			return returnNode;
		}
		else
			last = middle - 1;

		middle = (first + last)/2;
		if(searchWord!=NULL){
			free(searchWord);
			searchWord = NULL;
		}
	}
	if (first > last){
		return NULL;
	}
}

int getCell(char* lookupWord ,HashTable* hashTable,int previousBucket){
	Bucket *bucket = &hashTable->buckets[previousBucket];	
	
	int maxElems = bucket->position;
	int first = 0;
	int last = maxElems-1;
	int middle = (first+last)/2;
		
	char *searchWord=NULL;
	while (first <= last) {
		searchWord = getString(&bucket->cells[middle]);
		if (strcmp(searchWord,lookupWord)<0)
			first = middle + 1;    
		else if (strcmp(searchWord,lookupWord)==0) {
			free(searchWord);
			searchWord = NULL;
			return middle;
		}
		else
			last = middle - 1;

		middle = (first + last)/2;
		
		free(searchWord);
		searchWord = NULL;
		
	}
	if (first > last){
		if(searchWord!=NULL){
			free(searchWord);
			searchWord = NULL;
		}
		return -1;
	}
}



void destroyLinearHash(HashTable* hashTable){
	for(int i=0; i<hashTable->numberOfBuckets; i++){			//for each bucket
		Bucket* tempBucket=&hashTable->buckets[i];
		if(tempBucket!=NULL){
			for(int j=0; j<tempBucket->position; j++){		//for each cell
				deleteArray(tempBucket->cells[j].nextWordArray);	
				deleteDataNode(&(tempBucket->cells[j]));
			}
			free (tempBucket->cells);
			tempBucket->cells = NULL;
		}
	}
	free(hashTable->buckets);
	hashTable->buckets=NULL;
	free(hashTable);
	hashTable=NULL;
}

int getBucketFromHash(int level, int lengthHash, int bucketToBeSplit, char* word){	//word->int and by using h(level) returns number of bucket
	int charSum=0;
	for(int i=0; i < strlen(word); i++)				
		charSum+=(int)word[i];

	int hashLevel = charSum % ((int)pow(2,level) * lengthHash);
	int hashLevelPlus = charSum %  ((int)pow(2,level+1) * lengthHash);
	
	int a1 = (int)pow(2,level);
	int a2 = (int)pow(2,level+1);
	
	if(hashLevel>=bucketToBeSplit)
		return hashLevel;
	
	else
		return hashLevelPlus;
	


}
