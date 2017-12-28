#ifndef _STRUCT_H_
#define _STRUCT_H_
#include <stdbool.h>


typedef struct dataNode dataNode;

typedef struct arrayOfStructs arrayOfStructs;

struct dataNode{
	arrayOfStructs* nextWordArray;	//pointer to array of dataNodes
	char word[20];					//static word
	bool isFinal;					//final or not
	bool isDynamic;					//if true strlen(word)>20
	char* dynamicWord;				//at first is null
	int noOfChars;
	signed short* staticArray;		//for static files
	int staticArrayLength;			//for static files
	bool isEmpty;
};


struct arrayOfStructs{
	int length;				//dim of array initialized 10
	dataNode* array;		//pointer to array of structs dataNodes
	int position;			//the position of the first null element of array
};


void initializeArray(arrayOfStructs* array_of_str);
void doubleLength(arrayOfStructs* array_of_str);
void deleteArray(arrayOfStructs* array_of_str);
void deleteDataNode(dataNode* elem);
void initializeDataNode(dataNode* node);
void printStaticArray(dataNode* node);
void deleteArray1Layer(arrayOfStructs* array_of_str);

#endif
