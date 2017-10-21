#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "auxMethods.h"



int initialize(FILE* file){
	
	arrayOfStructs* array = (arrayOfStructs*) malloc(1 * sizeof(arrayOfStructs));
	array->length=10;
	array->position=0;
	initializeArray(array);

	char *line = NULL;
	size_t len = 0;
	char read;

	if (file == NULL)
		return 1;
	
	
	int insert=0;
	while ((read = getline(&line, &len, file)) != -1) {
	
		printf("%s",line);
	
		char* ngram = strtok(line, "\n");
		insert = stringToArray(ngram,array);
		
		if (!insert){
			fprintf( stderr, "%s\n","Insert was unsuccessful");	
			break;
		}

			
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

int stringToArray(char* ngram, arrayOfStructs* array){
	//printf ("===================================INSERT N GRAM IN NODE =====================================\n");
	char* pch;
	char** arrayOfWords; 
	//printf ("Splitting string %s! into tokens:\n",ngram);
	pch = strtok (ngram," ");
	int noOfWords=0;
	arrayOfWords = malloc(noOfWords * sizeof(char*));
	while (pch != NULL)
	{	
		noOfWords++;
		//printf ("!%s! noofwords = %d\n",pch,noOfWords);
		arrayOfWords = (char**)realloc(arrayOfWords, noOfWords * sizeof(char*));		
		arrayOfWords[noOfWords-1]=malloc(strlen(pch)* sizeof(char));
		strcpy(arrayOfWords[noOfWords-1],pch);    //add pch into arrayOfWords

		pch = strtok (NULL, " ");
		
	}
	
	if(!insert_ngram(array, arrayOfWords,noOfWords)){
		
		return 0;
	}
	
	
		

	return 1;
}




int binarySearch(int last, char* value, arrayOfStructs* array_of_str){
	int first, middle, previous;

	dataNode** array=array_of_str->array;

	first = 0;
	middle = (first+last)/2;
	
	if(last==0)
		return 0;
		
	while (first <= last) {
		previous = middle;
		

	
		if (strcmp(array[middle]->word, value)<0){
			//sprintf("if word in %d is %s\n", middle, array[middle]->word);
			
			first = middle + 1;
		}    
		else if (strcmp(array[middle]->word, value)==0) {
			//printf("else if %s found at location %d.\n", value, middle);
			return (middle);
		}
		else{
			//printf("else word in %d is %s\n", middle, array[middle]->word);
			last = middle - 1;
		}

		middle = (first + last)/2;
		if(previous==middle){
			//printf("%s should be at location %d.\n", value, middle);
			return (middle);
		}
			
		
	}
	if (first > last)
		printf("Not found! %s is not present in the list.\n", value);

	return 0;   
}

void printArray(arrayOfStructs* tempArray, int position){

	printf("ELEMENTS ARE: [");
	for(int k=0; k<= position;k++){
		printf("%s , ",  tempArray->array[k]->word);
	}
	printf("]\n");


}











