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


void initialize(FILE* file, arrayOfStructs* structureTree);
void executeQueryFile(FILE* file,arrayOfStructs* structureTree);

void callBasicFuncs(char* ngram, arrayOfStructs* array, char query);
arrayWords* stringToArray(char* ngram);

void printArray(arrayOfStructs* tempArray, int position);
void printArrayFinalWords(arrayOfStructs* array_of_str, int position);
void printFullArray(arrayOfStructs* array_of_str, int position);

checkItemExists* binarySearch(arrayOfStructs* array_of_str, dataNode* item, int first, int last, checkItemExists* check);

checkItemExists* insertionSort(arrayOfStructs* array_of_str, dataNode* itemForInsert, int lastElement);
void deletionSort(arrayOfStructs* array_of_str, int position, int lastElement);
void deleteArrayOfWords(char** array,int length);








int checkIfStringExists(char** array, int noOfWords, char* str);







#endif
