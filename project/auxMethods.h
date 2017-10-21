#ifndef _AUXMETHODS_H_
#define _AUXMETHODS_H_
#include "struct.h"
#include "func.h"

int initialize(FILE* file);
int stringToArray(char* ngram, arrayOfStructs* array);
int binarySearch(int last, char* value, arrayOfStructs* array_of_str);

void printArray(arrayOfStructs* tempArray, int position);


#endif
