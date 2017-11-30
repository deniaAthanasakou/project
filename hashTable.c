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

void deletionSortBucket(Bucket* bucket, int position){
	printf("start\n");
	printf("word %s noOfChars %d\n",getString(&bucket->cells[position]),bucket->cells[position].noOfChars);
	if(bucket->cells[position].dynamicWord!=NULL){	
		//printf("dynamic\n");	
		free(bucket->cells[position].dynamicWord);
		bucket->cells[position].dynamicWord=NULL;
	}
	else{
		bucket->cells[position].word[0] = '\0';
	}
	//printf("middle\n");
	if(bucket->cells[position].nextWordArray!=NULL){
		//printf("inside if\n");
		//printf("position %d\n",position);
		//printf("noOfChars %d\n",bucket->cells[position].noOfChars);
		//if(bucket->cells[position].nextWordArray!=NULL)
		//	printf("not null\n");
		printf("before delete position %d\n", position);
		deleteArray(bucket->cells[position].nextWordArray);
		printf("after delete\n");
		bucket->cells[position].nextWordArray=NULL;
	}
	
	int moveSize = 0;
	int startingPoint = position;
	while (position < bucket->position)
	{
	    //array_of_str->array[position]=array_of_str->array[position+1];
	    moveSize+=sizeof(bucket->cells[position+1]);
	    position++;
	}
	memmove(&(bucket->cells[startingPoint]),&(bucket->cells[startingPoint+1]), moveSize);
	bucket->position--;
}

dataNode* insertTrieNode(dataNode* node, HashTable* hashTable){
	char* word = getString(node);
	
	int noOfbucket = getBucketFromHash(hashTable->level, hashTable->initialLength, hashTable->bucketToBeSplit, word);
	//printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ INSERT trie word: '%s' in %d \n",word, noOfbucket);
	//printf("LENGTH OF HASHTABLE IS %d\n",hashTable->length);
	//printf("REAL LENGTH OF HASHTABLE IS %d\n",hashTable->numberOfBuckets);
	
	
	Bucket* bucket = &(hashTable->buckets[noOfbucket]);
	
	//printf("bucket->position %d\n",bucket->position);
	
	//insertion sort
	
	//printf("BEFORE INSERTIONSORT WITH WORD: %s\n",word);
	checkItemExists* check;
	//printf("wait\n");
	
	check = insertionSort2(hashTable, &(hashTable->buckets[noOfbucket]),node,bucket->position);
	//printf("AFTER	 INSERTIONSORT WITH WORD: %s\n",word);

	
	//printf("bucket pos %d and lngth: %d\n",bucket->position,bucket->length);
	//if(bucket->position == bucket->length){
	if(bucket->overflowed){
		//printf("WE NEED TO CHANGE SPLIT AND REARRANGE THE CELLS in bucket %d\n",hashTable->bucketToBeSplit);
		bucket->overflowed = false;
		hashTable->bucketToBeSplit++;
		splitBucket(hashTable);
	}
	//else{
	//	printf("not splited\n");
	//}
	
	free(word);
	word=NULL;
	

	 dataNode* returnNodePtr = check->insertedNode;
	
	if(check!=NULL){
		free(check);
		check=NULL;
	}
	
	

	//printf("before return\n");
	return returnNodePtr;
}



dataNode* insertTrieNodeAgain(dataNode* node, HashTable* hashTable, int checkBucketToBeSplit){
	//printf("insertTrieNodeAgain\n");
	char* word = getString(node);
	//printf("LENGTH OF HASHTABLE IS %d\n",hashTable->length);
	//printf("REAL LENGTH OF HASHTABLE IS %d\n",hashTable->numberOfBuckets);
	int noOfbucket = getBucketFromHash(hashTable->level, hashTable->initialLength, hashTable->bucketToBeSplit, word);
	//printf("----------------------INSERT trie again word: '%s' in %d  with position %d\n",word, noOfbucket,hashTable->buckets[noOfbucket].position);
	
	dataNode* splitted = NULL;
	if(checkBucketToBeSplit!=-1){
	//	printf("CHECK IF NEED TO DELETE\n");
		if(noOfbucket!=checkBucketToBeSplit){
		//	printf("DELETE PREVIOUS with checkBucketToBeSplit = %d\n",checkBucketToBeSplit);
			Bucket* temp = &(hashTable->buckets[checkBucketToBeSplit]);
			int prevPos = getCell(word,hashTable,checkBucketToBeSplit);
			splitted = malloc(sizeof(dataNode));
			initializeDataNode(splitted);
			printf("before splited\n");
			*splitted = hashTable->buckets[checkBucketToBeSplit].cells[prevPos];
			printf("after splited\n");
			//printFullArray(hashTable->buckets[checkBucketToBeSplit].cells[prevPos].nextWordArray,hashTable->buckets[checkBucketToBeSplit].cells[prevPos].nextWordArray->position);
			splitted->nextWordArray = hashTable->buckets[checkBucketToBeSplit].cells[prevPos].nextWordArray;
			deletionSortBucket(temp,prevPos);
			//printf("DELETION DONE\n");
			//printBuckets(temp);
		}
	}
	//printf("noOfbucket %d\n",noOfbucket);
	Bucket* bucket = &(hashTable->buckets[noOfbucket]);
	
	//insertion sort
	
	//printf("BEFORE INSERTIONSORT WITH WORD: %s\n",word);
	checkItemExists* check;
	//printf("wait\n");
	if(checkBucketToBeSplit==-1 || noOfbucket==checkBucketToBeSplit){
	//	printf("NORMAL INSERT\n");
		check = insertionSort2(hashTable, &(hashTable->buckets[noOfbucket]),node,bucket->position);
	//	printf("after insetionSort2\n");
	}
	else{
	//	printf("bucket was REARRANGED\n");
		if(noOfbucket!=checkBucketToBeSplit){
		//	printf("------insert to new bucket word : %s\n",word);
			check = insertionSort2(hashTable, &(hashTable->buckets[noOfbucket]),splitted,bucket->position);
		//	printf("after insetionSort2\n");
		}
		//if(splitted==NULL) printf("NOOOOOOOO\n");
		
	}
	
	

	free(word);
	word=NULL;
	 dataNode* returnNodePtr = check->insertedNode;
	
	if(check!=NULL){
		free(check);
		check=NULL;
	}

	//printf("before return\n");
	return returnNodePtr;
}



void createOverflowCells(Bucket* bucket){
	int oldLength = bucket->length;
	bucket->length += bucket->noOfCells;
	bucket->overflowed = true;
	//printf("OVERFLOW IS %d\n",bucket->length);
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
	printf("position %d\n",bucket->position);
	for(int i=0; i < bucket->position;i++){
		//break;
		char *word = getString(&bucket->cells[i]);
		printf("%dth elem with word: '%s'\n",i,word);
		if(bucket->cells[i].nextWordArray!=NULL)
			printFullArray(bucket->cells[i].nextWordArray, bucket->cells[i].nextWordArray->position);
		else
			printf("null next\n");	
		free(word);
		word = NULL;
	}
}

int levelUp(HashTable* hashTable){
	if(hashTable->numberOfBuckets == 2*(hashTable->length)){
		hashTable->level++;
		hashTable->length = 2*(hashTable->length);
		//printf("level is %d\n",hashTable->level);
		int oldBucket = hashTable->bucketToBeSplit -1;
		hashTable->bucketToBeSplit=0; 	//re-initialize bucketToBeSplit
		//printf("old Bucket is %d\n",oldBucket);
		return oldBucket;
	}
	return -1;	
		
}


void splitBucket(HashTable* hashTable){
	//printf("--------------------------------------SPLITTING BUCKET\n");
	hashTable->numberOfBuckets++;
	hashTable->buckets = realloc(hashTable->buckets, hashTable->numberOfBuckets * sizeof(Bucket));
	
	int nextCreatedBucket = hashTable->numberOfBuckets-1;
	//printf("%d bucket was initialized\n",nextCreatedBucket);
	initializeBucket(&hashTable->buckets[nextCreatedBucket], hashTable->buckets[0].noOfCells);
	int oldBucket = levelUp(hashTable);	
	
	if(oldBucket==-1){		//level wasn't increased
	//	printf("level up did not happen\n");
		oldBucket = hashTable->bucketToBeSplit -1;
	}	
	
	//for each dataNode in bucketToBeSplit call getHashBucket and rearrange
	//printf("old bucket%d\n", oldBucket);
	Bucket *tempBucket = &(hashTable->buckets[oldBucket]);
	//printf("tempBucket->position %d\n",tempBucket->position);
	
	printBucket(tempBucket);
	printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
	for(int i=0; i<tempBucket->position; i++){		//for each cell of bucket

		//printBuckets(tempBucket);	
	//	printf("-----------------------------------REARRANGE '%s'\n",tempBucket->cells[i].word);
		char* oldWord=  getString(&tempBucket->cells[i]);
		//printf("oldBucket %d\n",oldBucket);
		insertTrieNodeAgain(&tempBucket->cells[i],hashTable,oldBucket);
		
		int newBucket = getBucketFromHash(hashTable->level,hashTable->initialLength, hashTable->bucketToBeSplit,oldWord);
		//printf("newBucket %d oldBucket %d\n",newBucket,oldBucket);
		if(newBucket!=oldBucket){	//node was moved to another bucket
			i--;
		}
		//printf("after insertTrie %s\n",oldWord);
		
	}
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
	//printf("inside lookup\n");
	Bucket *bucket = &hashTable->buckets[previousBucket];	
	
	int maxElems = bucket->position;
	int first = 0;
	int last = maxElems-1;
	int middle = (first+last)/2;
		
	char *searchWord=NULL;
	//printf("word CELLLL: %s\n",lookupWord);
	while (first <= last) {
		searchWord = getString(&bucket->cells[middle]);
	//	printf("WORDDDDDD IN WHILE: %s\n",searchWord);
		if (strcmp(searchWord,lookupWord)<0)
			first = middle + 1;    
		else if (strcmp(searchWord,lookupWord)==0) {
		//	printf("FOUND with word: %s\n",searchWord);
			free(searchWord);
			searchWord = NULL;
			return middle;
		}
		else
			last = middle - 1;

		middle = (first + last)/2;
	}
	if (first > last){
		//printf("kk\n");
		if(searchWord!=NULL){
			free(searchWord);
			//printf("ll\n");
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
				//deletionSortBucket(tempBucket, j);
				deleteDataNode(&(tempBucket->cells[j]));
			}
			free (tempBucket->cells);
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
	
	//printf("lengthHash = %d a1 %d a2 %d\n",lengthHash, a1, a2);
	
	//printf("word '%s' charSum '%d' bucketToBeSplit '%d' hashLevel '%d' hashLevelPlus '%d'\n",word, charSum, bucketToBeSplit,hashLevel,hashLevelPlus);
	//return 0;
	if(hashLevel>=bucketToBeSplit)
		return hashLevel;
	
	else
		return hashLevelPlus;
	


}
