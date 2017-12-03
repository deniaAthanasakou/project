#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "topK.h"
#define TOP_SIZE 100
#define BUCKETS 10

topKArray* initializeTopKArray(){
	topKArray *topArray = malloc(sizeof(topKArray));
	topArray->length = TOP_SIZE;
	topArray->positionInsertion = 0;
	topArray->array = malloc(topArray->length*sizeof(topKStruct));
}


void doubleTopKArray(topKArray *topArray){
	topArray->length = 2*topArray->length;
	topArray->array = realloc(topArray->array,topArray->length*sizeof(topKStruct));
}

void insertTopArray(topKArray *topArray,char *item){
	if(item!=NULL){
		topArray->array[topArray->positionInsertion].ngram = malloc((strlen(item)+1)*sizeof(char));
		topArray->array[topArray->positionInsertion].occurences = 1;
		strcpy(topArray->array[topArray->positionInsertion].ngram,item);
		topArray->positionInsertion++;
	}
}

//array: array of strings, n: number of elems(positionInsertion)
void bucketSort(topKArray *topArray) {
    int i, j, k;
	topKStruct **buckets = malloc(BUCKETS * sizeof(topKStruct*));
	int n = topArray->positionInsertion;
	
	//initialization
    for(i = 0; i < BUCKETS; ++i){
        buckets[i] = NULL;
	}
    
	//insert values to buckets depending on occurences
	/*
    for(i = 0; i < n; ++i){
		
		topKStruct *tempNode = malloc(sizeof(topKStruct));
		tempNode->ngram = malloc(strlen((topArray->array[i].ngram)+1)*sizeof(char));
		strcpy(tempNode->ngram,topArray->array[i].ngram);
		tempNode->occurences = topArray->array[i].occurences;
		
        ++buckets[a[i]];
	}
      
	
    for(i = 0, j = 0; j < BUCKETS; ++j){
        for(k = buckets[j]; k > 0; --k){
            a[i++] = j;
		}
	}*/
}

void pigeonholeSort(topKArray *topArray){
	int size, count = 0, i;
	topKStruct *a = topArray->array;
    topKStruct *current = a;
	int max = topArray->array[topArray->positionInsertion-1].occurences;
	int min = topArray->array[0].occurences;
    size = max-min + 1;
	
	//initialize
    topKStruct *holes[size];
    for (i = 0; i < size; i++)
    {
		holes[i] = malloc(sizeof(topKStruct));
    }
	
	//insert values to each pigeon
    for (i = 0; i < size; i++, current++)
    {
        holes[current->occurences-min] += 1;
    }
	
	
    for (count = 0, current = &a[0]; count < size; count++)
    {
        while (holes[count]-- > 0)
        {
            *current = *holes[count + min];
        }
    }


}

void printTopK(topKArray *topArray,int topK){
	printf("Top: ");
	for(int i=0; i < topK; i++){
		if(i!=topK-1){
			printf("%s|",topArray->array[topK].ngram);
		}
		else{
			printf("%s\n",topArray->array[topK].ngram);
		}
	}
}

void destroyTopArray(topKArray *topArray){
	for(int i=0; i <= topArray->positionInsertion;i++){
		if(topArray->array[i].ngram!=NULL){
			free(topArray->array[i].ngram);
			topArray->array[i].ngram = NULL;
		}
	}
	if(topArray->array!=NULL){
		free(topArray->array);
		topArray->array = NULL;
	}
	if(topArray!=NULL){
		free(topArray);
		topArray = NULL;
	}
}