#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "func.h"
#include "auxMethods.h"
#include "bloomfilter.h"
#include "hashTable.h" 

#define NOOFBUCKETS 4
#define NOOFCELLS 4


int main (int argc,char* argv[]){
	
	if(argc!=5){
		printf("Error! Wrong Number of Arguments\n");
		exit(1);
	}
	char* init=NULL;
	char* query=NULL;
	for(int i=0; i<argc; i++){
		if(!strcmp(argv[i],"-i"))
			init=argv[i+1];							//argument before initialize file should be -i
		else if(!strcmp(argv[i],"-q"))
			query=argv[i+1];						//argument before query file should be -q
	}
	
	if(init==NULL || query==NULL){
		printf("Error! Wrong Arguments\n");
		exit(1);
	}
	
	
	arrayOfStructs* structureTree = (arrayOfStructs*) malloc(1 * sizeof(arrayOfStructs));
	initializeArray(structureTree);
	
	HashTable* hashTable = createLinearHash(NOOFBUCKETS, NOOFCELLS);
	
	
	
	//initialize
	FILE * initFile;
	initFile = fopen (init,"r");
	int staticDynamic = 1;
	if (initFile!=NULL)
	{
		staticDynamic = initialize(initFile, structureTree, hashTable);
		fclose (initFile);
	}
	//printBuckets(hashTable);
	//deletionSortBucket(&hashTable->buckets[0],1);
	
	//printBuckets(hashTable);
	//printBucket(&hashTable->buckets[0]);
	//query
	FILE * queryFile;
	queryFile = fopen (query,"r");
	if (queryFile!=NULL)
	{
		int query = executeQueryFile(queryFile,structureTree, hashTable, staticDynamic);
		if(query==0){
			printf("Error in file of queries\n");
			exit(1);
		}
		fclose (queryFile);
	}
	
	return 0;
}
