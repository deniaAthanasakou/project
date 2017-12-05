#ifndef _AUXMETHODS_H_
#define _AUXMETHODS_H_
#include "struct.h"
#include "func.h"
#include "bloomfilter.h"
#include "hashTable.h"
#include "topK.h"

typedef struct checkItemExists checkItemExists;

struct checkItemExists{
	int position;	//position of element to insert
	bool exists;	// if exists true
	dataNode* insertedNode;
};

typedef struct arrayWords{
	int length;					//length of array of strings
	char** words;			//array of strings
}arrayWords;



int initialize(FILE* file, HashTable* hashTable);
int executeQueryFile(FILE* file, HashTable* hashTable, int staticDynamic);

void callBasicFuncs(char* ngram, char query , HashTable* hashTable, BloomFilter* topFilter, topKArray *topArray, int isDynamic);

arrayWords* stringToArray(char* ngram);

void printArray(arrayOfStructs* tempArray, int position);
void printArrayFinalWords(arrayOfStructs* array_of_str, int position);
void printFullArray(arrayOfStructs* array_of_str, int position);

int checkIfStringExists(char** array, int noOfWords, char* str);

checkItemExists* binarySearch(arrayOfStructs* array_of_str, dataNode* item, int first, int last, checkItemExists* check);

checkItemExists* insertionSort(arrayOfStructs* array_of_str, dataNode* itemForInsert, int lastElement);
void deletionSort(arrayOfStructs* array_of_str, int position, int lastElement);
void deleteArrayOfWords(char** array,int length);
void printQuery(char** items, int iterNum);

char* getString(dataNode* node);
void insertString (dataNode* node, char* word);



checkItemExists* binarySearch2(dataNode* array, dataNode* item, int first, int last, int trueLastElement, checkItemExists* check);

checkItemExists* insertionSort2(HashTable* hashTable, Bucket* bucket, dataNode* itemForInsert, int lastElement);




void copyDataNode(dataNode* node, dataNode* tempNode);


#endif
