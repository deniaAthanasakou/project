#ifndef _FUNC_H_
#define _FUNC_H_
#include <stdbool.h>
#include "struct.h"
#include "auxMethods.h"


//insert
void insert_ngram(arrayOfStructs* array_of_structs, char** arrayOfWords, int noOfWords);

//search
void search_ngram(arrayOfStructs* array_of_structs, char** arrayOfWordsOriginal, int noOfWordsOriginal);

//delete
void delete_ngram(arrayOfStructs* array_of_structs, char** arrayOfWords, int noOfWords);


#endif
