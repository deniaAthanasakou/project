#ifndef _NODE_H_
#define _NODE_H_
#include <stdbool.h>

typedef struct dataNode dataNode;

struct dataNode{
	dataNode* nextWord;
	char* word;
	bool isFinal;
	
};


int insert_ngram_in_node(char* ngram);
void setNode(dataNode* this,dataNode* nw,char* data, bool f);


#endif
