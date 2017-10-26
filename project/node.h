#ifndef _NODE_H_
#define _NODE_H_
#include <stdbool.h>

typedef struct dataNode dataNode;

struct dataNode{
	dataNode** nextWordArray;		//pointer se array apo dataNodes
	char* word;						//leksh
	bool isFinal;					//final h oxi
};


int stringToArray(char* ngram);
void setNode(dataNode* this,dataNode* nw,char* data, bool f);


#endif
