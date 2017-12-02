#ifndef _HASH_TABLE_H_
#define _HASH_TABLE_H_
#include "struct.h"


typedef struct HashTable HashTable;
typedef struct Bucket Bucket;
typedef struct BucketCell BucketCell;

struct HashTable{
	int length;					//m
	int initialLength; 
	Bucket* buckets;			//array of buckets
	int level;					//level of hashing
	int bucketToBeSplit;		//p
	int numberOfBuckets;		//real length of bukets
};

struct Bucket{
	int length;					
	dataNode* cells;			//array of struct datanode
	int position;				//position of last element
	int noOfCells;				//c initial length
	bool overflowed;
};


HashTable* createLinearHash(int noOfBuckets, int noOfCells);
void destroyLinearHash(HashTable* hashTable);
dataNode* insertTrieNode(dataNode* node, HashTable* hashTable);
dataNode* insertTrieNodeAgain(dataNode* node, HashTable* hashTable, int checkBucketToBeSplit);	
dataNode* lookupTrieNode(char* lookupWord ,HashTable* hashTable);

int deleteTrieNode(char* deleteWord ,HashTable* hashTable);

void initializeBucket(Bucket* bucket, int noOfCells);


int getBucketFromHash(int level, int lengthHash, int bucketToBeSplit, char* word);

dataNode* splitBucket(HashTable* hashTable, char* oldCellWord);
int levelUp(HashTable* hashTable);
void deletionSortBucket(Bucket* bucket, int position);
void printBucket(Bucket *bucket);
void printBuckets(HashTable* hashTable);

void createOverflowCells(Bucket* bucket);
int getCell(char* lookupWord ,HashTable* hashTable,int previousBucket);

void deletionSortWithoutErasingNextArray(Bucket* bucket, int position);

#endif
