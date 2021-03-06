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
int destroyLinearHash(HashTable* hashTable);
int insertTrieNode(dataNode* node, HashTable* hashTable);
dataNode* lookupTrieNode(char* lookupWord ,HashTable* hashTable);

int deleteTrieNode(char* deleteWord ,HashTable* hashTable);

void initializeBucket(Bucket* bucket, int noOfCells, int noOfElements );


int getBucketFromHash(int level, int lengthHash, int bucketToBeSplit, char* word);


#endif
