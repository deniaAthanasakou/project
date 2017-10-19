#ifndef _LIST_H_
#define _LIST_H_
#include <stdbool.h>
#include "node.h"

typedef struct arrayHeader{
	int length;				//diastash array
	dataNode** array;		//pointer se array apo dataNodes
}arrayHeader;

int insert_ngram(dataNode* node);




#endif
