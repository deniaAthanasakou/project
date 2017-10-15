#ifndef _LIST_H_
#define _LIST_H_
#include <stdbool.h>
#include "node.h"

typedef struct listNode listNode;

struct node{
	listNode* sameLayer;
	dataNode* node;
	
};

int insert_ngram(dataNode* node);




#endif
