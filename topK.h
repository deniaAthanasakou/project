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
void insertTopArray(topKArray *topArray,char *item);
void printTopK(topKArray *topArray,int topK);
void destroyTopArray(topKArray *topArray);
void bucketSort(topKArray *topArray);
void pigeonholeSort(topKArray *topArray);

#endif