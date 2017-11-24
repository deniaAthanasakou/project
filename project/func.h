#ifndef _FUNC_H_
#define _FUNC_H_
#include <stdbool.h>
#include "struct.h"
#include "auxMethods.h"
#include "bloomfilter.h"


//insert
void insert_ngram(arrayOfStructs* array_of_structs, char** arrayOfWords, int noOfWords);

//search
char* search_ngram(arrayOfStructs* array_of_structs, char** arrayOfWordsOriginal, int noOfWordsOriginal);

//delete
void delete_ngram(arrayOfStructs* array_of_structs, char** arrayOfWords, int noOfWords);


#endif
