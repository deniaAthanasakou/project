#ifndef _AUXMETHODS_H_
#define _AUXMETHODS_H_
#include "struct.h"
#include "func.h"

int initialize(FILE* file);
int stringToArray(char* ngram, arrayOfStructs* array);

void printArray(arrayOfStructs* tempArray, int position);

int binarySearch(arrayOfStructs* array_of_str,dataNode* item, int first, int last);
void insertionSort(arrayOfStructs* array_of_str, int n);


#endif
