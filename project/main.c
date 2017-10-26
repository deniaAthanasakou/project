#include <stdio.h>
#include <stdlib.h>
#include "func.h"
#include "auxMethods.h"



//Run with ./ngrams -i init_file -q query_file


int main (int argc,char* argv[]){
	
	if(argc!=5){
		printf("Error! Wrong Arguments\n");
		exit(1);
	}
	char* init=argv[2];
	char* query=argv[4];
	
	arrayOfStructs* structureTree = (arrayOfStructs*) malloc(1 * sizeof(arrayOfStructs));
	
	//initialize
	FILE * initFile;
	initFile = fopen (init,"r");
	if (initFile!=NULL)
	{
		printf ("Before initializeList\n");
		int init = initialize(initFile, structureTree);
		printf ("After initializeList\n");
		if (!init){			//error with initializeList
			exit(1); 
		}
		fclose (initFile);
	}
	
	//query
	/*FILE * queryFile;
	queryFile = fopen (query,"r");
	if (queryFile!=NULL)
	{
		printf ("Before query\n");
		int query = executeQueryFile(queryFile);
		printf ("After query\n");
		if (!query){			//error with query
			exit(1); 
		}
		fclose (queryFile);
	}
*/

	return 0;
}
