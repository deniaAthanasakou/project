#ifndef _TOP_K_H_
#define _TOP_K_H_

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

void HeapSort(topKStruct* array, int length, int stringFlag);
void BuildHeap(topKStruct* array, int* heapSize, int length, int stringFlag);
void HeapifyIntegers(topKStruct* array, int i, int* heapSize);
void HeapifyStrings(topKStruct* array, int i, int* heapSize);



void binarySearchTopK(topKStruct* array, char* ngram, int maxElems);
//int binarySearchTopK(topKStruct* array, int l, int r, char* ngram);

#endif
