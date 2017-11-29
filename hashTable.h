#ifndef _HASH_TABLE_H_
#define _HASH_TABLE_H_
#include "struct.h"


typedef struct HashTable HashTable;
typedef struct Bucket Bucket;

struct HashTable{
	int length;					//m
	Bucket* buckets;			//array of buckets
	int level;					//level of hashing
	int bucketToBeSplit;		//p
	int numberOfBuckets;		//real length of bukets
};

struct Bucket{
	int length;					//c
	dataNode* cells;			//array of struct datanode
	Bucket* nextBucket;			//ptr for overflow
	//int hashFunc;	
	int position;				//position of last element
	int noOfElements;
};

HashTable* createLinearHash(int noOfBuckets, int noOfCells);
void destroyLinearHash(HashTable* hashTable);
dataNode* insertTrieNode(dataNode* node, HashTable* hashTable, int checkBucketToBeSplit);		//if checkBucketToBeSplit==-1 then insertTrieNode was called from insert_ngram
dataNode* lookupTrieNode(char* lookupWord ,HashTable* hashTable);

int deleteTrieNode(char* deleteWord ,HashTable* hashTable);

void initializeBucket(Bucket* bucket, int noOfCells, int noOfElements );


int getBucketFromHash(int level, int lengthHash, int bucketToBeSplit, char* word);

void splitBucket(HashTable* hashTable);
void levelUp(HashTable* hashTable);
Bucket* lookupBucket(char *lookupWord,HashTable *hashTable);
void deletionSortBucket(Bucket* bucket, int position);
void printBuckets(Bucket *bucket);

#endif