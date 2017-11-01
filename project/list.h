#ifndef _LIST_H_
#define _LIST_H_
#include <stdbool.h>

typedef struct node listNode;

struct node{
	listNode* sameLayer;
	listNode* nextWord;
	char* word;
	bool isfinal;
	
};

int insert_ngram(char* ngram);




#endif
