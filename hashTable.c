#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "hashTable.h"
#include "auxMethods.h"

HashTable* createLinearHash(int noOfBuckets, int noOfCells){
	HashTable* hashTable = malloc(sizeof(HashTable));
	hashTable->buckets = malloc(noOfBuckets * sizeof(Bucket));
	hashTable->length = noOfBuckets;
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

dataNode* insertTrieNode(dataNode* node, HashTable* hashTable, int checkBucketToBeSplit){
	char* word = getString(node);
	int noOfbucket = getBucketFromHash(hashTable->level, hashTable->length, hashTable->bucketToBeSplit, word);
	//printf("INSERT word: '%s' in %d \n",word, noOfbucket);
	
	dataNode* splitted = NULL;
	if(checkBucketToBeSplit!=-1){
		//printf("CHECK IF NEED TO DELETE\n");
		if(noOfbucket!=checkBucketToBeSplit){
			//printf("DELETE PREVIOUS with checkBucketToBeSplit = %d\n",checkBucketToBeSplit);
			Bucket* temp = &(hashTable->buckets[checkBucketToBeSplit]);
			//printBuckets(temp);
			int prevPos = getCell(word,hashTable,checkBucketToBeSplit);
			splitted = malloc(sizeof(dataNode));
			*splitted = hashTable->buckets[checkBucketToBeSplit].cells[prevPos];
			deletionSortBucket(temp,prevPos);
			//printf("DELETION DONE\n");
			//printBuckets(temp);
		}
	}
	
	Bucket* bucket = &(hashTable->buckets[noOfbucket]);
	if(bucket->position==bucket->length && checkBucketToBeSplit==-1){
		//printf("WE NEED TO CHANGE SPLIT AND REARRANGE THE CELLS\n");
		hashTable->bucketToBeSplit++;
		splitBucket(hashTable);
	}
	
	//insertion sort
	
	//printf("BEFORE INSERTIONSORT WITH WORD: %s\n",word);
	checkItemExists* check;
	if(checkBucketToBeSplit==-1){
		//printf("NORMAL INSERT\n");
		check = insertionSort2(hashTable, &(hashTable->buckets[noOfbucket]),node,bucket->position);
	}
	else{
		//printf("SPLITTED\n");
		if(noOfbucket!=checkBucketToBeSplit){
			//printf("------insert to new bucket word : %s\n",word);
			check = insertionSort2(hashTable, &(hashTable->buckets[noOfbucket]),splitted,bucket->position);
		}
		//if(splitted==NULL) printf("NOOOOOOOO\n");
		
	}
	free(word);
	word=NULL;
	
	//printf("--------------OTH BUCKET-----------------\n");
	//printBuckets(&hashTable->buckets[0]);
	/*printf("--------------4TH BUCKET-----------------\n");
	printBuckets(&hashTable->buckets[4]);*/
	
  //  dataNode* returnNodePtr;
	 dataNode* returnNodePtr = check->insertedNode;
	
	
	//dataNode returnNode = *(check->insertedNode);
	if(check!=NULL){
		/*if(check->insertedNode!=NULL){
			deleteDataNode(check->insertedNode);
			free(check->insertedNode);
		}*/
		free(check);
		check=NULL;
	}
	
	//return returnNodePtr;
	return returnNodePtr;
}

void createOverflowCells(Bucket* bucket){
	bucket->length += bucket->noOfCells;
	bucket->cells = realloc(bucket->cells,bucket->noOfCells * sizeof(dataNode));
}


void printBuckets(Bucket *bucket){
	//printf("position %d\n",bucket->position);
	for(int i=0; i < bucket->position;i++){
		char *word = getString(&bucket->cells[i]);
		printf("%dth elem with word: '%s'\n",i,word);
		//if(bucket->cells[i].nextWordArray!=NULL)
		//	printFullArray(bucket->cells[i].nextWordArray,bucket->cells[i].nextWordArray->position);
		free(word);
		word = NULL;
	}
}

void levelUp(HashTable* hashTable){
	if(hashTable->length = 2*hashTable->length)
		hashTable->level++;
}


void splitBucket(HashTable* hashTable){
	//printf("SPLITTING BUCKET\n");
	hashTable->numberOfBuckets++;
	hashTable->buckets = realloc(hashTable->buckets, hashTable->numberOfBuckets * sizeof(Bucket));
	
	int nextCreatedBucket = hashTable->numberOfBuckets-1;
	initializeBucket(&hashTable->buckets[nextCreatedBucket], hashTable->buckets[0].noOfCells);
	levelUp(hashTable);		//mhpws bgazei sega
	
	//for each dataNode in bucketToBeSplit call getHashBucket and rearrange
	Bucket *tempBucket = &(hashTable->buckets[hashTable->bucketToBeSplit-1]);
	for(int i=0; i<tempBucket->position; i++){		//for each cell of bucket

			
		//printf("-----------------------------------REARRANGE '%s'\n",tempBucket->cells[i].word);
		 insertTrieNode(&tempBucket->cells[i],hashTable,hashTable->bucketToBeSplit-1);
		int newBucket = getBucketFromHash(hashTable->level, hashTable->length, hashTable->bucketToBeSplit, getString(&tempBucket->cells[i]));
		//printf("newBucket %d oldBucket %d\n",newBucket,hashTable->bucketToBeSplit-1);
		if(newBucket!=hashTable->bucketToBeSplit-1){	//node was moved to another bucket
			i--;
		}
		/*if(check!=NULL){
			if(check->insertedNode!=NULL){
				deleteDataNode(check->insertedNode);
				free(check->insertedNode);
				check->insertedNode = NULL;
			}
			free(check);
			check=NULL;
		}*/
	}
}


dataNode* lookupTrieNode(char* lookupWord ,HashTable* hashTable){
	int getCorrectBucket = getBucketFromHash(hashTable->level, hashTable->length, hashTable->bucketToBeSplit, lookupWord);
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
	
	//printf("word '%s' charSum '%d' bucketToBeSplit '%d'\n",word, charSum, bucketToBeSplit);
	
	if(hashLevel>=bucketToBeSplit)
		return hashLevel;
	
	else
		return hashLevelPlus;
	


}
