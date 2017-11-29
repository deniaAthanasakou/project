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

void deletionSortBucket(Bucket* bucket, int position){
	printf("INSIDE DELETION SORT WITH POS: %d\n",position);
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
	printf("IN DELSORT BEFORE\n");
	printBuckets(bucket);
	memmove(&(bucket->cells[startingPoint]),&(bucket->cells[startingPoint+1]), moveSize);
	
	bucket->position--;
	bucket->noOfElements--;
	printf("IN DELSORT AFTER\n");
	printBuckets(bucket);
}

dataNode* insertTrieNode(dataNode* node, HashTable* hashTable, int checkBucketToBeSplit){
	char* word = getString(node);
	int noOfbucket = getBucketFromHash(hashTable->level, hashTable->length, hashTable->bucketToBeSplit, word);
	
	
	printf("INSERT word: %s in %d \n",word, noOfbucket);
	
	Bucket* bucket = &(hashTable->buckets[noOfbucket]);
	
	printf("got right bucket %d \n",noOfbucket);

	if(&(hashTable->buckets[noOfbucket])==NULL)
		printf("NULL SPLIT BUCKET\n");	
	else
		printf("NOT NULL %d\n",	hashTable->buckets[noOfbucket].noOfElements);

	//insertion sort
	checkItemExists* check = insertionSort2(hashTable, &(hashTable->buckets[noOfbucket]),node, hashTable->buckets[noOfbucket].noOfElements);
	printf("a \n");
	if(!check->exists){	
		printf("++++++++checkBucketToBeSplit is %d\n",checkBucketToBeSplit);
		
		printf("b \n");
	/*Bucket* bucketOfItem = lookupBucket("tihs",hashTable);
		if(bucketOfItem!=NULL){
			printf("not null with word %s\n",getString(&(bucketOfItem->cells[0])));	
			printf("not null with word %s\n",getString(&(bucketOfItem->cells[1])));	
			
		}
		else
			printf("null\n");*/
		
		if(checkBucketToBeSplit!=-1){
			if(noOfbucket!=checkBucketToBeSplit){
				printf("old bucket = %d new bucket = %d\n",checkBucketToBeSplit,noOfbucket);
				//bucket will be changed
				//Bucket* bucketOfItem = lookupBucket(word,hashTable);
				BucketCell* bucketCell = lookupBucket(word,hashTable);
				//if(bucketOfItem!=NULL){
					printf("printing bucket before deletion sort\n");
					printBuckets(&(hashTable->buckets[checkBucketToBeSplit]));
				
				
					printf("word in lookup is '%s'\n",hashTable->buckets[checkBucketToBeSplit].cells[bucketCell->cellNum].word);
					printf("----------position to delete : %d\n ",bucketCell->cellNum);
					deletionSortBucket(&(hashTable->buckets[checkBucketToBeSplit]),bucketCell->cellNum);
					printf("word in lookup is '%s'\n",hashTable->buckets[checkBucketToBeSplit].cells[bucketCell->cellNum].word);
				//}
				//else{
				//	printf("not found\n");
				
				//}
			}
		}
		
		hashTable->buckets[noOfbucket].noOfElements++;
		
		while(bucket!=NULL){					//go to last overflowed bucket
			if(bucket->position==bucket->length){	//overflow
				if(bucket->nextBucket==NULL){
					bucket->nextBucket = malloc(sizeof(Bucket));
					initializeBucket(bucket->nextBucket,bucket->length,bucket->noOfElements);					
				}	
			}
			if(bucket->position==1 && bucket!=&(hashTable->buckets[noOfbucket])){
				hashTable->bucketToBeSplit++;
				//split bucket in position hashTable->bucketToBeSplit
				splitBucket(hashTable);
				printf("INITIALIZE BUCKET 4\n");
			}
			bucket=bucket->nextBucket;
		}
	}
	free(word);
	word=NULL;
	
	//printf("--------------OTH BUCKET-----------------\n");
	//printBuckets(&hashTable->buckets[0]);
	/*printf("--------------4TH BUCKET-----------------\n");
	printBuckets(&hashTable->buckets[4]);*/
	
    /*dataNode* returnNodePtr;
	*returnNodePtr = *check->insertedNode;
	
	
	//dataNode returnNode = *(check->insertedNode);
	if(check!=NULL){
		if(check->insertedNode!=NULL){
			deleteDataNode(check->insertedNode);
			free(check->insertedNode);
		}
		free(check);
		check=NULL;
	}*/
	
	//return returnNodePtr;
	return check->insertedNode;
}

void printBuckets(Bucket *bucket){
	while(bucket!=NULL){
		printf("bucket->position %d\n",bucket->position);
		for(int i=0; i < bucket->position;i++){
			char *word = getString(&bucket->cells[i]);
			printf("%dth elem with word: '%s'\n",i,word);
			free(word);
			word = NULL;
		}
		bucket = bucket->nextBucket;
	}
}

void levelUp(HashTable* hashTable){
	hashTable->length*=2;
	hashTable->level++;
}

void splitBucket(HashTable* hashTable){
	printf("SPLITTING BUCKET\n");
	hashTable->numberOfBuckets++;
	hashTable->buckets = realloc(hashTable->buckets, hashTable->numberOfBuckets * sizeof(Bucket));
	int nextCreatedBucket = hashTable->numberOfBuckets-1;
	initializeBucket(&hashTable->buckets[nextCreatedBucket], hashTable->buckets[0].length,0);
	
	//for each dataNode in bucketToBeSplit call getHashBucket and rearrange
	Bucket *tempBucket = &(hashTable->buckets[hashTable->bucketToBeSplit -1 ]);
	while(tempBucket!=NULL){							//for each connected bucket
		printf("splittinggggggg pos is : %d\n",tempBucket->position);
		for(int i=0; i<tempBucket->position; i++){		//for each cell of bucket
			
			printf("SALKDJASKDJASJDAJ  '%s' in cell %d\n",tempBucket->cells[i].word,i);
			insertTrieNode(&tempBucket->cells[i],hashTable,hashTable->bucketToBeSplit-1);
			printf("new is '%s'\n",hashTable->buckets[4].cells[0].word);
			printf("old is '%s'\n",hashTable->buckets[0].cells[i].word);
		}
		tempBucket = tempBucket->nextBucket;
	}
	

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


BucketCell* lookupBucket(char *lookupWord,HashTable *hashTable){
	BucketCell* bucketCell = malloc(sizeof(BucketCell));
	bucketCell->bucket=NULL;
	bucketCell->cellNum = -1;
	int getCorrectBucket = getBucketFromHash(hashTable->level, hashTable->length, hashTable->bucketToBeSplit, lookupWord);
	int maxElems = hashTable->buckets[getCorrectBucket].noOfElements+1;
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
			return bucketCell;
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
	
	printf("before array\n");
	for(int i=0; i<arrayCounter; i++){
		printf("---------------%dth elem with %s\n",i,searchArray[i]->word);
	}
	printf("after array\n");
	
	int first = 0;
	int last = maxElems - 1;
	int middle = (first+last)/2;
	
	//printf("first: %d, last: %d, middle: %d\n",first,last,middle);
	
	char *searchWord;
	while (first <= last) {
		searchWord = getString(searchArray[middle]);
		//printf("searchword is '%s'\n",searchArray[middle]->word);
		if (strcmp(searchWord,lookupWord)<0)
			first = middle + 1;    
		else if (strcmp(searchWord,lookupWord)==0) {
			printf("AAAAAAAAAAAAA: found in pos %d\n",middle);
			printf("searchWord %s\n",searchWord);
			free(searchWord);
			searchWord = NULL;
			free(searchArray);
			searchArray = NULL;
			
			printf("AAAAAAAAAAAAA\n");
			Bucket* tempBucket = &(hashTable->buckets[ getCorrectBucket]);
			printf("BBBBBBB\n");
			int numBucket=(middle-1)/hashTable->buckets[0].length;
			printf("numBucket = %d\n",numBucket);
			for(int k=0; k<numBucket; k++){
				tempBucket= tempBucket->nextBucket;
			}
			
			bucketCell->bucket=tempBucket;
			bucketCell->cellNum = (middle+1)% hashTable->buckets[0].length;
			
			
			return bucketCell;
			//return copyBucket;
		}
		else
			last = middle - 1;

		middle = (first + last)/2;
	}
	if (first > last){
		//printf("FUCKKKKKKKKK\n");
		free(searchWord);
		searchWord = NULL;
		free(searchArray);
		searchArray = NULL;
		
		bucketCell->bucket=NULL;
		bucketCell->cellNum = -1;
		
		return bucketCell;
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
