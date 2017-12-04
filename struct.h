#ifndef _STRUCT_H_
#define _STRUCT_H_
#include <stdbool.h>


typedef struct dataNode dataNode;

typedef struct arrayOfStructs arrayOfStructs;

struct dataNode{
	arrayOfStructs* nextWordArray;	//pointer se array apo dataNodes
	char word[20];						//leksh
	bool isFinal;					//final or not
	bool isDynamic;					//if true strlen(word)>20
	char* dynamicWord;				//at first is null
	int noOfChars;
	int* staticArray;				//for static files
};


struct arrayOfStructs{
	int length;				//diastash array initialized 10
	dataNode* array;		//pointer se array apo structs dataNodes
	int position;			//the position of the first null element of array
};



void initializeArray(arrayOfStructs* array_of_str);
void doubleLength(arrayOfStructs* array_of_str);
void deleteArray(arrayOfStructs* array_of_str);
void deleteDataNode(dataNode* elem);
void initializeDataNode(dataNode* node);

#endif
