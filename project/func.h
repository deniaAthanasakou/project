#ifndef _LIST_H_
#define _LIST_H_
#include <stdbool.h>
#include "struct.h"
#include "auxMethods.h"


//insert
int insert_ngram(arrayOfStructs* array_of_structs, char** arrayOfWords, int noOfWords);

//search
int search_ngram(arrayOfStructs* array_of_structs, char** arrayOfWordsOriginal, int noOfWordsOriginal);

//delete


#endif
