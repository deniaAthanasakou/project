#ifndef _FUNC_H_
#define _FUNC_H_
#include <stdbool.h>
#include "struct.h"
#include "bloomfilter.h"
#include "hashTable.h"
#include "auxMethods.h"
#include "topK.h"


//insert
void insert_ngram(HashTable* hashTable, char** arrayOfWords, int noOfWords, int additionNum);

//search
char* search_ngram(void** args);

//search in static files
char* search_ngram_StaticVersion(void** args);

//delete
void delete_ngram(HashTable* hashTable, char** arrayOfWords, int noOfWords, int deletionNum);


#endif
