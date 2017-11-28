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

dataNode* insertTrieNode(dataNode* node, HashTable* hashTable){
	char* word = getString(node);
	int noOfbucket = getBucketFromHash(hashTable->level, hashTable->length, hashTable->bucketToBeSplit, word);
	//printf("INSERT word: %s\n",word);
	Bucket* bucket = &(hashTable->buckets[noOfbucket]);
	

	//insertion sort
	checkItemExists* check = insertionSort2(hashTable, &(hashTable->buckets[noOfbucket]),node, hashTable->buckets[noOfbucket].noOfElements);
	
	if(!check->exists){	
		//printf("INSERTed word: %s\n",word);
		hashTable->buckets[noOfbucket].noOfElements++;
		int i=0;
		while(bucket!=NULL){					//go to last overflowed bucket
			printf("%d %s\n",i, node->word);
			if(bucket->position==bucket->length){	//overflow
				if(bucket->nextBucket==NULL){
					bucket->nextBucket = malloc(sizeof(Bucket));
					initializeBucket(bucket->nextBucket,bucket->length,bucket->noOfElements);
					//hashTable->bucketToBeSplit++;
					
				}	
				
			}
			
			if(bucket->position==1 && bucket!=&(hashTable->buckets[noOfbucket])){
				//split bucket in position hashTable->bucketToBeSplit
				hashTable->bucketToBeSplit++;
			}
			bucket=bucket->nextBucket;
			i++;
			
			//printf("hashTable->bucketToBeSplit %d\n",hashTable->bucketToBeSplit);
		}
	}
	free(word);
	word=NULL;
	/*dataNode returnNode = *(check->insertedNode);
	if(check->insertedNode!=NULL){
		deleteDataNode(check->insertedNode);
		free(check->insertedNode);
	}
	
	free(check);
	check=NULL;
	dataNode* returnNodePtr = &returnNode;
	return returnNodePtr;*/
	return check->insertedNode;
}

void levelUp(HashTable* hashTable){
	hashTable->length*=2;
	hashTable->level++;
}

void splitBucket(HashTable* hashTable, int bucketToBeSplit){
	

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


dataNode* lookupTrieNode(char* lookupWord ,HashTable* hashTable){
	//printf("inside lookup\n");
	int getCorrectBucket = getBucketFromHash(hashTable->level, hashTable->length, hashTable->bucketToBeSplit, lookupWord);
	int maxElems = hashTable->buckets[getCorrectBucket].noOfElements;
	dataNode** searchArray = malloc(maxElems * sizeof(dataNode*));
	Bucket *copyBucket = &hashTable->buckets[getCorrectBucket];
	Bucket *firstBucket = copyBucket;
	int arrayCounter=0;
	int cellCounter=0;
	while(copyBucket!=NULL){		
		if(cellCounter == copyBucket->length){		//max cell
			copyBucket = copyBucket->nextBucket;
			cellCounter = 0;
		}
		if(copyBucket==NULL)
			break;
		
		if(firstBucket==copyBucket  && copyBucket->position == 0) {
			return NULL;
		}
		
		int position = copyBucket->position;
		if(cellCounter >= position) {
			break;
		}	
		
		
		//printf("arrayCounter = %d,cellCounter = %d\n ",arrayCounter,cellCounter);
		searchArray[arrayCounter] = &copyBucket->cells[cellCounter];
		//printf("---------------%dth elem with %s\n",arrayCounter,searchArray[arrayCounter]->word);
		arrayCounter++;
		cellCounter++;
	
	}
	
	/*printf("after array\n");
	for(int i=0; i<arrayCounter; i++){
		printf("---------------%dth elem with %s\n",i,searchArray[i]->word);
	}*/
	
	int first = 0;
	int last = maxElems - 1;
	int middle = (first+last)/2;
	
	//printf("middle:  %d\n",middle);
	
	char *searchWord;
	while (first <= last) {
		searchWord = getString(searchArray[middle]);
		//printf("searchword is '%s'\n",searchArray[middle]->word);
		if (strcmp(searchWord,lookupWord)<0)
			first = middle + 1;    
		else if (strcmp(searchWord,lookupWord)==0) {
			free(searchWord);
			searchWord = NULL;
			dataNode* returnNode =  searchArray[middle];
			free(searchArray);
			searchArray = NULL;
			return returnNode;
		}
		else
			last = middle - 1;

		middle = (first + last)/2;
	}
	if (first > last){
		free(searchWord);
		searchWord = NULL;
		free(searchArray);
		searchArray = NULL;
		//printf("nulllll in lookup\n");
		return NULL;
	}

}





void destroyLinearHash(HashTable* hashTable){
	for(int i=0; i<hashTable->length; i++){			//for each bucket
		Bucket* tempBucket=&hashTable->buckets[i];
		while(tempBucket!=NULL){					//for each overflow bucket
			for(int j=0; j<tempBucket->position; j++){		//for each cell
				deleteArray(tempBucket->cells[j].nextWordArray);
				deletionSortBucket(tempBucket, j);
			}
					
		
			tempBucket = tempBucket->nextBucket;
		}
		
		free (hashTable->buckets[i].cells);

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
