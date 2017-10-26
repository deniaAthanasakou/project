#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "auxMethods.h"



int initialize(FILE* file){
	
	structureTree = (arrayOfStructs*) malloc(1 * sizeof(arrayOfStructs));
	structureTree->length=10;
	structureTree->position=0;
	initializeArray(structureTree);

	char *line = NULL;
	size_t len = 0;
	char read;

	if (file == NULL)
		return 1;
	
	
	int insert=0;
	while ((read = getline(&line, &len, file)) != -1) {
	
		printf("%s",line);
	
		char* ngram = strtok(line, "\n");
		
		//find if first word is A for insert
		char* wordCase = strtok(ngram," ");
		if(strcmp(wordCase,"A")==0){
			printf("INSERT\n");
		}
		
		insert = stringToArray(ngram,structureTree);
		
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


int binarySearch(arrayOfStructs* array_of_str, dataNode* item, int first, int last)
{

	dataNode** array=array_of_str->array;

    if (last <= first)
        return (strcmp(item->word,array[first]->word)>0)?  (first + 1): first;

 	int mid = (first+last)/2;
 
    if(strcmp(item->word ,array[mid]->word)==0)
        return mid+1;
 
    if(strcmp(item->word,array[mid]->word)>0)
        return binarySearch(array_of_str, item, mid+1, last);
    return binarySearch(array_of_str, item, first, mid-1);
}
 
 
 
// Function to sort an array_of_str->array  of size 'n'
void insertionSort(arrayOfStructs* array_of_str, int n)
{
    int i, loc, j, k;
    dataNode* selected;
 
    for (i = 1; i < n; i++)
    {
        j = i - 1;
        selected = array_of_str->array[i];
 
        // find location where selected sould be inseretd
        loc = binarySearch(array_of_str, selected, 0, j);
 
        // Move all elements after location to create space
        while (j >= loc)
        {
            array_of_str->array[j+1] = array_of_str->array[j];
            j--;
        }
        array_of_str->array[j+1] = selected;
    }
}




void printArray(arrayOfStructs* array_of_str, int position){

	printf("ELEMENTS ARE: [");
	for(int k=0; k<= position;k++){

		printf("%s ",  array_of_str->array[k]->word);
		
		if(k!=position)
			printf(", ");
	}
	printf("]\n");


}


//search for the same word of array in arrayOfStructs
int binarySearchSame(arrayOfStructs* array_of_str,char*word,int position){

	int first = 0;
	int last = position-1;
	dataNode** array=array_of_str->array; 
	int middle = (first+last)/2;

	while (first <= last) {
		if (strcmp(array[middle]->word,word) < 0)
			first = middle + 1;    
		else if (strcmp(array[middle]->word,word) > 0) {
			last = middle - 1;
		}
		else
			return middle;

		middle = (first + last)/2;
	}
	if (first > last)
		return -1;

}

//insert from query file
int executeQueryFile(FILE* file){

	char *line = NULL;
	size_t len = 0;
	char read;

	if (file == NULL)
		return 1;
	
	
	int insert=0;
	while ((read = getline(&line, &len, file)) != -1) {
	
		printf("%s",line);
	
		char* ngram = strtok(line, "\n");
		
		//find first letter for each case
		char* wordCase = strtok(ngram," ");
		char* remainingLine = strtok(NULL,"");
		//printf("remaining:%s\n",remainingLine);
		if(strcmp(wordCase,"A")==0){
			printf("INSERT\n");
			insert = stringToArray(remainingLine,structureTree);
			if (!insert){
				fprintf( stderr, "%s\n","Insert was unsuccessful");	
				break;
			}
		}
		else if(strcmp(wordCase,"Q")==0){
			printf("SEARCH\n");
		}
		else if(strcmp(wordCase,"D")==0){
			printf("DELETE\n");
		}
		else if(strcmp(wordCase,"F")==0){
			printf("END\n");
		}
		else{
			printf("Error in query_file structure.\n");
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










