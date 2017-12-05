#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "topK.h"


topKArray* initializeTopKArray(){
	topKArray *topArray = malloc(sizeof(topKArray));
	topArray->length = TOP_SIZE;
	topArray->positionInsertion = 0;
	topArray->array = malloc(topArray->length*sizeof(topKStruct));
	for(int i=0; i<topArray->length; i++){
		 initializeTopKStruct(&topArray->array[i]);
	}
	return topArray;
}


void doubleTopKArray(topKArray *topArray){
	int oldLength = topArray->length;
	topArray->length *= 2;
	topArray->array = (topKStruct*) realloc(topArray->array, topArray->length*sizeof(topKStruct));
	for(int i=oldLength; i<topArray->length; i++){
		 initializeTopKStruct(&topArray->array[i]);
	}
}

void initializeTopKStruct(topKStruct* node){
	node->ngram = NULL;
	node->occurences = 0;
}

void insertTopArray(topKArray *topArray,char *item){
	if(item!=NULL){
		topArray->array[topArray->positionInsertion].ngram = malloc((strlen(item)+1)*sizeof(char));
		topArray->array[topArray->positionInsertion].occurences = 1;
		strcpy(topArray->array[topArray->positionInsertion].ngram,item);
		topArray->positionInsertion++;
	}
}


void HeapifyStrings(topKStruct* array, int i, int* heapSize)
{
	int l = 2 * i + 1;
	int r = 2 * i + 2;
	int largest;


	if(l <= *heapSize && strcmp(array[l].ngram, array[i].ngram)>0){
		largest = l;
	}
	else
		largest = i;
		
	if(r <= *heapSize &&  strcmp(array[r].ngram, array[largest].ngram)>0){
		largest = r;
	}	
		
	if(largest != i)
	{
		topKStruct temp = array[i];
		array[i] = array[largest];
		array[largest] = temp;
		HeapifyStrings(array, largest,heapSize);
	}
}



void HeapifyIntegers(topKStruct* array, int i, int* heapSize)
{
	int l = 2 * i + 1;
	int r = 2 * i + 2;
	int largest;

	if(l <= *heapSize && array[l].occurences < array[i].occurences)
		largest = l;
	else if(l <= *heapSize && array[l].occurences == array[i].occurences){
		if(strcmp(array[l].ngram, array[i].ngram)>0){
			largest = l;
		}
		else
			largest = i;
	}
	else
		largest = i;
		
	if(r <= *heapSize && array[r].occurences < array[largest].occurences)
		largest = r;
	else if(r <= *heapSize && array[r].occurences == array[largest].occurences){
		if(strcmp(array[r].ngram, array[largest].ngram)>0){
			largest = r;
		}
	}
		
	if(largest != i)
	{
		topKStruct temp = array[i];
		array[i] = array[largest];
		array[largest] = temp;
		HeapifyIntegers(array, largest,heapSize);
	}
}

void BuildHeap(topKStruct* array, int* heapSize, int length, int stringFlag)
{
	*heapSize = length - 1;

	for(int i = (length - 1) / 2; i >= 0; i--){
		if(stringFlag == 0)
			HeapifyIntegers(array, i, heapSize);
		else
			HeapifyStrings(array, i, heapSize);
	}
}

void HeapSort(topKStruct* array, int length, int stringFlag)
{
	int heapSize = length - 1;	
	BuildHeap(array, &heapSize, length,stringFlag);
	
	for(int i = length - 1; i > 0; i--)
	{
		topKStruct temp = array[heapSize];
		array[heapSize] = array[0];
		array[0] = temp;
		heapSize--;
		if(stringFlag == 0)
			HeapifyIntegers(array, 0, &heapSize);
		else
			HeapifyStrings(array, 0, &heapSize);	
	}
}

void binarySearchTopK(topKStruct* array, char* ngram, int maxElems){
	int first = 0;
	int last = maxElems - 1;
	int middle = (first+last)/2;
	//printf("last = %d\n",last);
	while (first <= last) {
		//printf("MID = %d\n", middle);
		if (strcmp(array[middle].ngram,ngram)<0)
			first = middle + 1;    
		else if (strcmp(array[middle].ngram,ngram)==0) {		//found increase occurences
			array[middle].occurences++;
			//printf("FINAL STRING EXISTS\n");
			return;
		}
		else
			last = middle - 1;

		middle = (first + last)/2;
	}
	//printf("FINAL STRING DOES NOT EXIST\n");
}




void printTopK(topKArray *topArray,int topK){
	printf("Top: ");
	for(int i=0; i < topK; i++){
		if(i!=topK -1){
			printf("%s|",topArray->array[i].ngram);
		}
		else{
			printf("%s\n",topArray->array[i].ngram);
		}
	}
}

void printFullArrayTop(topKArray *topArray){
	printf("Printing Array: ");
	for(int i=0; i < topArray->positionInsertion; i++){
		if(i!=topArray->positionInsertion-1){
			printf("%s: %d|",topArray->array[i].ngram,topArray->array[i].occurences);
		}
		else{
			printf("%s %d\n",topArray->array[i].ngram,topArray->array[i].occurences);
		}
	}
}


void destroyTopArray(topKArray *topArray){
	if(topArray!=NULL){
		if(topArray->array!=NULL){
			for(int i=0; i < topArray->positionInsertion;i++){
				if(topArray->array[i].ngram!=NULL){
					free(topArray->array[i].ngram);
					topArray->array[i].ngram = NULL;
				}
				topArray->array[i].occurences = -1;
			}
		
			free(topArray->array);
			topArray->array = NULL;
		}
	
		free(topArray);
		topArray = NULL;
	}
}
