#include <stdio.h>
#include <stdlib.h>

#include "hashTable.h"

HashTable* createLinearHash(int noOfBuckets, int noOfCells){
	HashTable* hashTable = malloc(noOfBuckets * sizeof(Bucket));
	hashTable->length = noOfBuckets;
	hashTable->level = 0;
	for(int i=0; i<noOfBuckets; i++){
		hashTable->buckets[i].length = noOfCells;
		hashTable->buckets[i].nextBucket = NULL;
		hashTable->buckets[i].cells = malloc(noOfCells * sizeof(dataNode));
	}
	
	return hashTable;
}


int destroyLinearHash(HashTable* hashTable){
	for(int i=0; i<hashTable->length; i++){			//for each bucket
		for(int j=0; j<hashTable->buckets[i].noOfCells; j++){		//for each cell
			//while exei next phgaine ekei
			//deleteDataNode
			//free(node)
			
		}
		//free (hashTable->buckets[i].cells)

	}
	//free(buckets)
	//free(hashTable)
	return 0;
}
