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


void insertTopArray(topKArray *topArray,char *item)
{	
	int lastElement = topArray->positionInsertion;
	
	topKStruct* itemForInsert = malloc(sizeof(topKStruct));
	itemForInsert->ngram = malloc((strlen(item)+1)*sizeof(char));
	itemForInsert->occurences = 1;
	strcpy(itemForInsert->ngram,item);
	
 	if(lastElement==0){
 		topArray->array[lastElement] = * itemForInsert;
		topArray->positionInsertion++;
		return;
	}
    // find location where selected sould be inseretd
    int loc = binarySearchTopK(topArray->array, item, 0, topArray->positionInsertion-1);
	int j = lastElement -1; 
	
	
	int fullMoveSize=0;
	int startingPoint = loc;	
	if( j>=loc){
		fullMoveSize = (lastElement -loc)*sizeof(topArray->array[lastElement - 1]);
		memmove(&(topArray->array[startingPoint+1]), &(topArray->array[startingPoint]), fullMoveSize);
		memmove(&(topArray->array[loc]), itemForInsert, sizeof(*itemForInsert));
	}
	else{
		memmove(&(topArray->array[loc]), itemForInsert, sizeof(*itemForInsert));
	}	
    
	topArray->positionInsertion++;

}



void Heapify(topKStruct* array, int i, int* heapSize)
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
		Heapify(array, largest,heapSize);
	}
}

void BuildHeap(topKStruct* array, int* heapSize, int length)
{
	*heapSize = length - 1;

	for(int i = (length - 1) / 2; i >= 0; i--){
		Heapify(array, i, heapSize);
	}
}

void HeapSort(topKStruct* array, int length, int topK)
{
	int heapSize = length - 1;	
	BuildHeap(array, &heapSize, length);
	int counter=0;
	for(int i = length - 1; i > 0; i--)
	{
		topKStruct temp = array[heapSize];
		array[heapSize] = array[0];
		array[0] = temp;
		heapSize--;
		Heapify(array, 0, &heapSize);
		counter++;
		/*if(counter==topK)
			break;*/
	}
}

void swap(topKStruct *xp, topKStruct *yp)
{
    topKStruct temp = *xp;
    *xp = *yp;
    *yp = temp;
}
 
// A function to implement bubble sort
void bubbleSort(topKStruct* array, int length, int topK)
{
   int i, j;
   for (i = 0; i < length; i++)      
 
       // Last i elements are already in place   
       for (j = 0; j < length-i-1; j++) {
           if (array[j].occurences < array[j+1].occurences)
              swap(&array[j], &array[j+1]); 
       }    		
}

int binarySearchTopK(topKStruct* array, char* item, int low, int high){

    if (high <= low){
    	if(strcmp(item,array[low].ngram)>0){
    		return low+1;
    	}
    	return low;	
    }
    int mid = (low + high)/2;
 
    if(strcmp(item,array[mid].ngram)==0){
 		return mid;
 	}
    if(strcmp(item,array[mid].ngram)>0){
        return binarySearchTopK(array, item, mid+1, high);
    }
    return binarySearchTopK(array, item, low, mid-1);
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
