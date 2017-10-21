#ifndef _NODE_H_
#define _NODE_H_
#include <stdbool.h>

typedef struct dataNode dataNode;

typedef struct arrayOfStructs arrayOfStructs;

struct dataNode{
	arrayOfStructs* nextWordArray;	//pointer se array apo dataNodes
	char* word;						//leksh
	bool isFinal;					//final or not
};


struct arrayOfStructs{
	int length;				//diastash array initialized 10
	dataNode** array;		//pointer se array apo dataNodes
	int position;			//the position of the first null element of array
};



void initializeArray(arrayOfStructs* array_of_str);
void doubleLength(arrayOfStructs* array_of_str);


#endif
