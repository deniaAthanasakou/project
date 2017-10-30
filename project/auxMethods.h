#ifndef _AUXMETHODS_H_
#define _AUXMETHODS_H_
#include "struct.h"
#include "func.h"

typedef struct checkItemExists checkItemExists;

struct checkItemExists{
	int position;	//position of element to insert
	bool exists;	// if exists true
};


int initialize(FILE* file, arrayOfStructs* structureTree);
int executeQueryFile(FILE* file,arrayOfStructs* structureTree);

int stringToArray(char* ngram, arrayOfStructs* array, char query);

void printArray(arrayOfStructs* tempArray, int position);
void printArrayFinalWords(arrayOfStructs* array_of_str, int position);

checkItemExists* binarySearch(arrayOfStructs* array_of_str, dataNode* item, int first, int last);

checkItemExists* insertionSort(arrayOfStructs* array_of_str, dataNode* itemForInsert, int lastElement);
void deletionSort(arrayOfStructs* array_of_str, int position, int lastElement);
void deleteArrayOfWords(char** array,int length);

#endif
