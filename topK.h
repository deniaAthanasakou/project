#ifndef _TOP_K_H_
#define _TOP_K_H_
#define TOP_SIZE 20
#include <stdbool.h>

typedef struct topKStruct{
	char* ngram;
	int occurences;
}topKStruct;

typedef struct topKArray{
	int length;
	topKStruct *array;
	int positionInsertion;		//empty position for insert
}topKArray;

topKArray* initializeTopKArray();
void doubleTopKArray(topKArray *topArray);
void initializeTopKStruct(topKStruct* node);
void insertTopArray(topKArray *topArray,char *item);
void printTopK(topKArray *topArray,int topK);
void printFullArrayTop(topKArray *topArray);
void destroyTopArray(topKArray *topArray);

void HeapSort(topKStruct* array, int length, int topK);
void BuildHeap(topKStruct* array, int* heapSize, int length);
void Heapify(topKStruct* array, int i, int* heapSize);

int binarySearchTopK(topKStruct* array, char* item, int low, int high);


void bubbleSort(topKStruct* array, int length, int topK);
void swap(topKStruct *xp, topKStruct *yp);

#endif
