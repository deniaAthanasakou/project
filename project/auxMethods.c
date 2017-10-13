#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "auxMethods.h"
#include "list.h"


int initializeList(FILE* file){
	//while not eof
	//pairnei grammh
	// thn kanei add
	//end of while

	char *line = NULL;
	size_t len = 0;
	char read;

	if (file == NULL)
		return 1;
	
	int insert=0;
	while ((read = getline(&line, &len, file)) != -1) {
	
	
		char* ngram = strtok(line, "\n");
		insert = insert_ngram(ngram);
		if (!insert){
			fprintf( stderr, "%s\n","Insert was unsuccessful");	
			break;
		}
			
		else
			printf("successful insert %s with length %d\n",line,read);
			
	}
	//found eof

	if (line){
		free(line);
		line=NULL;
	}
	
		
	if (!insert)
		return 0;
	
		
	
	
	return 1;

}
