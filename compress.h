#ifndef _COMPRESS_H_
#define _COMPRESS_H_
#include "struct.h"
#include "auxMethods.h"

void recreateStructure(HashTable* hashTable);
void compress(dataNode* startNode,dataNode* additionalNode);

#endif
