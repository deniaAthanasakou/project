#ifndef _FUNC_H_
#define _FUNC_H_
#include <stdbool.h>
#include "struct.h"
#include "bloomfilter.h"
#include "hashTable.h"

typedef struct topKStruct{
	char* ngram;
	int occurences;
}topKStruct;

//insert
void insert_ngram(HashTable* hashTable, char** arrayOfWords, int noOfWords);

//search
char* search_ngram(HashTable *hashTable, char** arrayOfWordsOriginal, int noOfWordsOriginal, BloomFilter* topFilter, topKStruct* topKArray);

//delete
void delete_ngram(HashTable* hashTable, char** arrayOfWords, int noOfWords);


#endif
