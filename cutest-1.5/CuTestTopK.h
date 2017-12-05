#ifndef CU_TEST_TOP_K_H
#define CU_TEST_TOP_K_H
#include "CuTest.h"
#include "../topK.h"

void TestInitializeTopKArray(CuTest *tc);
void TestInitializeTopKStruct(CuTest *tc);
/*
void doubleTopKArray(topKArray *topArray);
void insertTopArray(topKArray *topArray,char *item);
void printTopK(topKArray *topArray,int topK);
void printFullArrayTop(topKArray *topArray);
void destroyTopArray(topKArray *topArray);

void HeapSort(topKStruct* array, int length, int stringFlag);
void BuildHeap(topKStruct* array, int* heapSize, int length, int stringFlag);
void HeapifyIntegers(topKStruct* array, int i, int* heapSize);
void HeapifyStrings(topKStruct* array, int i, int* heapSize);

void binarySearchTopK(topKStruct* array, char* ngram, int maxElems);
*/

CuSuite* TopKGetSuite();

#endif

