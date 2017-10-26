#include <stdio.h>
#include <stdlib.h>
#include "func.h"
#include "auxMethods.h"

int main (int argc,char* argv[]){
	
	//initialize
	FILE * initFile;
	initFile = fopen ("init_file","r");
	if (initFile!=NULL)
	{
		printf ("Before initializeList\n");
		int init = initialize(initFile);
		printf ("After initializeList\n");
		if (!init){			//error with initializeList
			exit(1); 
		}
		fclose (initFile);
	}
	
	//query
	FILE * queryFile;
	queryFile = fopen ("query_file","r");
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


	return 0;
}
