#include <stdio.h>
#include <stdlib.h>
#include "func.h"
#include "auxMethods.h"

int main (int argc,char* argv[]){
	
	
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
	


	return 0;
}
