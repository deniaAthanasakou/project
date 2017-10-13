#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "auxMethods.h"

int main (int argc,char* argv[]){

	//pairnei path tou init
	//anoigei file
	
	// kleinei file
	
	
	FILE * initFile;
	initFile = fopen ("init_file","r");
	if (initFile!=NULL)
	{
		printf ("fopen example\n");
		int init = initializeList(initFile);
		if (!init){			//error with initializeList
			exit(1); 
		}
		fclose (initFile);
	}
	


	return 0;
}
