#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "func.h"
#include "auxMethods.h"
#include "test.h"



//Run with ./ngrams -i init_file -q query_file


int main (int argc,char* argv[]){
	
	if(argc!=5){
		printf("Error! Wrong Number of Arguments\n");
		exit(1);
	}
	char* init=NULL;
	char* query=NULL;
	for(int i=0; i<argc; i++){
		if(!strcmp(argv[i],"-i"))
			init=argv[i+1];
		else if(!strcmp(argv[i],"-q"))
			query=argv[i+1];	
	}
	
	if(init==NULL || query==NULL){
		printf("Error! Wrong Arguments\n");
		exit(1);
	}
	
	printf("\nstarting testing\n");
	
	testAllFunctions(); //test that all functions are working properly
	
	printf("ending testing\n\n\n");
	
	arrayOfStructs* structureTree = (arrayOfStructs*) malloc(1 * sizeof(arrayOfStructs));
	initializeArray(structureTree);
	
	//initialize
	FILE * initFile;
	initFile = fopen (init,"r");
	if (initFile!=NULL)
	{
		//printf ("Before initializeList\n");
		initialize(initFile, structureTree);
		fclose (initFile);
	}
	
	
	
	//printf("\n\n\nPRINTING FULL TREE before query\n\n\n");
	
	//printFullArray(structureTree,structureTree->position);
	//printf("\n\n");
	//query
	FILE * queryFile;
	queryFile = fopen (query,"r");
	if (queryFile!=NULL)
	{
		//printf ("Before query\n");
		int query = executeQueryFile(queryFile,structureTree);
		if(query==0){
			printf("Error in file of queries\n");
			exit(1);
		}
		fclose (queryFile);
		//printf("queryfile after\n");
	}
//	printf("\n\n\nPRINTING FULL TREE after query\n\n\n");
	
	//printFullArray(structureTree,structureTree->position);
	
	//printf("\n\n\nBEFORE DELETE\n\n\n");
	//deleteArray(structureTree);
	//structureTree=NULL;

	return 0;
}
