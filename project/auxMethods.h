#ifndef _AUXMETHODS_H_
#define _AUXMETHODS_H_
#include "struct.h"
#include "func.h"

typedef struct checkItemExists checkItemExists;

struct checkItemExists{
	int position;	//position of element to insert
	bool exists;	// if exists true
};

typedef struct arrayWords{
	int length;					//length of array of strings
	char** words;			//array of strings
}arrayWords;


int initialize(FILE* file, arrayOfStructs* structureTree);
int executeQueryFile(FILE* file,arrayOfStructs* structureTree);

int callBasicFuncs(char* ngram, arrayOfStructs* array, char query);
arrayWords* stringToArray(char* ngram, arrayOfStructs* array);

void printArray(arrayOfStructs* tempArray, int position);
void printArrayFinalWords(arrayOfStructs* array_of_str, int position);
void printFullArray(arrayOfStructs* array_of_str, int position);

checkItemExists* binarySearch(arrayOfStructs* array_of_str, dataNode* item, int first, int last);

checkItemExists* insertionSort(arrayOfStructs* array_of_str, dataNode* itemForInsert, int lastElement);
void deletionSort(arrayOfStructs* array_of_str, int position, int lastElement);
void deleteArrayOfWords(char** array,int length);

#endif
