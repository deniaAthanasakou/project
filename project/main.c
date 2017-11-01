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
	
	/*arrayOfStructs* structureTree = (arrayOfStructs*) malloc(1 * sizeof(arrayOfStructs));
	
	//initialize
	FILE * initFile;
	initFile = fopen (init,"r");
	if (initFile!=NULL)
	{
		//printf ("Before initializeList\n");
		int init = initialize(initFile, structureTree);
		//printf ("After initializeList\n");
		if (!init){			//error with initializeList
			exit(1); 
		}
		fclose (initFile);
	}
	
	//query
	FILE * queryFile;
	queryFile = fopen (query,"r");
	if (queryFile!=NULL)
	{
		//printf ("Before query\n");
		int query = executeQueryFile(queryFile,structureTree);
		//printf ("After query\n");
		if (!query){			//error with query
			//printf("query =0\n");
			exit(1); 
		}
		//printf("queryfile before\n");
		fclose (queryFile);
		//printf("queryfile after\n");
	}
	printf("\n\n\nPRINTING FULL TREE\n\n\n");
	
	printFullArray(structureTree,structureTree->position);
	
	printf("\n\n\nBEFORE DELETE\n\n\n");
	deleteArray(structureTree);*/

	return 0;
}
