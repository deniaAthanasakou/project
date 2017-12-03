#ifndef _FUNC_H_
#define _FUNC_H_
#include <stdbool.h>
#include "struct.h"
#include "bloomfilter.h"
#include "hashTable.h"
#include "auxMethods.h"
#include "topK.h"


//insert
void insert_ngram(HashTable* hashTable, char** arrayOfWords, int noOfWords);

//search
char* search_ngram(HashTable *hashTable, char** arrayOfWordsOriginal, int noOfWordsOriginal, BloomFilter* topFilter, topKArray *topArray);

//delete
void delete_ngram(HashTable* hashTable, char** arrayOfWords, int noOfWords);


#endif
