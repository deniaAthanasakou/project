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
	hashTable->level = 0;
	hashTable->bucketToBeSplit = 0;
	for(int i=0; i<noOfBuckets; i++){			//for each bucket
		initializeBucket(&(hashTable->buckets[i]),noOfCells,0);
	}
	return hashTable;
}

void initializeBucket(Bucket* bucket, int noOfCells, int noOfElements ){
	bucket->length = noOfCells;
	bucket->nextBucket = NULL;
	bucket->position = 0;
	bucket->cells = malloc(noOfCells * sizeof(dataNode));
	bucket->noOfElements = noOfElements;
	
}

int insertTrieNode(dataNode* node, HashTable* hashTable){

	char* word = getString(node);
	int noOfbucket = getBucketFromHash(hashTable->level, hashTable->length, hashTable->bucketToBeSplit, word);

	Bucket* bucket = &(hashTable->buckets[noOfbucket]);
	while(bucket!=NULL){					//go to last overflowed bucket
		if(bucket->position==bucket->length){	//overflow
			initializeBucket(bucket->nextBucket,bucket->length,bucket->noOfElements);
		
		}
		bucket=bucket->nextBucket;
	}
	
	
	//insertion sort
	checkItemExists* check = insertionSort2(&(hashTable->buckets[noOfbucket]),node, hashTable->buckets[noOfbucket].noOfElements);
	if(!check->exists){
	
		printf("inserted item '%s' in position '%d'\n",word,check->position);
	
		hashTable->buckets[noOfbucket].noOfElements++;
	}
	
	
}






int destroyLinearHash(HashTable* hashTable){
	for(int i=0; i<hashTable->length; i++){			//for each bucket
		for(int j=0; j<hashTable->buckets[i].length; j++){		//for each cell
			//while exei next phgaine ekei
			//deleteDataNode
			//free(node)
			
		}
		//free (hashTable->buckets[i].cells)

	}
	//free(buckets)
	//free(hashTable)
	return 0;
}

int getBucketFromHash(int level, int lengthHash, int bucketToBeSplit, char* word){	//word->int and by using h(level) returns number of bucket
	int charSum=0;
	for(int i=0; i < strlen(word); i++)				
		charSum+=(int)word[i];

	int hashLevel = charSum % ((int)pow(2,level) * lengthHash);
	int hashLevelPlus = charSum %  ((int)pow(2,level+1) * lengthHash);
	
	printf("word '%s' charSum '%d' bucketToBeSplit '%d'\n",word, charSum, bucketToBeSplit);
	
	if(hashLevel>=bucketToBeSplit)
		return hashLevel;
	
	else
		return hashLevelPlus;
	


}
