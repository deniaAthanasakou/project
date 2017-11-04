#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "func.h"
#include "auxMethods.h"
#include "test.h"


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
	
	printf("\nTESTING FUNCTIONS\n");
	
	testAllFunctions(); //test that all functions are working properly
	
	printf("TESTING FUNCTIONS ENDED\n\n\n");
	
	arrayOfStructs* structureTree = (arrayOfStructs*) malloc(1 * sizeof(arrayOfStructs));
	initializeArray(structureTree);
	
	//initialize
	FILE * initFile;
	initFile = fopen (init,"r");
	if (initFile!=NULL)
	{
		initialize(initFile, structureTree);
		
		fclose (initFile);
	}
	
	//query
	FILE * queryFile;
	queryFile = fopen (query,"r");
	if (queryFile!=NULL)
	{
		int query = executeQueryFile(queryFile,structureTree);
		if(query==0){
			printf("Error in file of queries\n");
			printf("\n");
			exit(1);
		}
		fclose (queryFile);
	}
	printf("\n");

	return 0;
}
