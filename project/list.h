#ifndef _LIST_H_
#define _LIST_H_
#include <stdbool.h>

typedef struct node listNode;

struct node{
	listNode* sameLayer;
	listNode* nextWord;
	char* word;
	bool isFinal;
	
};

int insert_ngram(char* ngram);
void setNode(listNode* this, listNode* sl,listNode* nw,char* data, bool f);




#endif
