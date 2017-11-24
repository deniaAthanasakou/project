#ifndef _HASH_TABLE_H_
#define _HASH_TABLE_H_
#include "struct.h"

typedef struct HashTable HashTable;
typedef struct Bucket Bucket;

struct HashTable{
	int length;				//m
	Bucket* buckets;		//array of buckets
	int level;				//level of hashing
};

struct Bucket{
	int length;					//c
	dataNode* cells;		//array of struct datanode
	Bucket* nextBucket;			//ptr for overflow
};

HashTable* createLinearHash(int noOfBuckets, int noOfCells);
int destroyLinearHash(HashTable* hashTable);
int insertTrieNode(dataNode* node, HashTable* hashTable);
dataNode* lookupTrieNode(char* lookupWord ,HashTable* hashTable);

int deleteTrieNode(char* deleteWord ,HashTable* hashTable);

#endif
