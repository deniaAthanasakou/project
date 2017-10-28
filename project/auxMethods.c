#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "auxMethods.h"



int initialize(FILE* file, arrayOfStructs* structureTree){
	

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
		
		
		insert = stringToArray(ngram,structureTree,'A');
		
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

int stringToArray(char* ngram, arrayOfStructs* array, char query){
	//printf ("===================================INSERT N GRAM IN NODE =====================================\n");
	char* pch;
	char** arrayOfWords; 
	pch = strtok (ngram," ");
	int noOfWords=0;
	arrayOfWords = malloc(noOfWords * sizeof(char*));
	while (pch != NULL)
	{	
		noOfWords++;
		arrayOfWords = (char**)realloc(arrayOfWords, noOfWords * sizeof(char*));		
		arrayOfWords[noOfWords-1]=malloc(strlen(pch)* sizeof(char));
		strcpy(arrayOfWords[noOfWords-1],pch);    //add pch into arrayOfWords

		pch = strtok (NULL, " ");
		
	}
	
	if(query == 'A'){
		if(!insert_ngram(array, arrayOfWords,noOfWords)){
			return 0;
		}	
	}
	if(query == 'Q'){
		if(!search_ngram(array, arrayOfWords,noOfWords)){
			return 0;
		}	
	}
	
	

	return 1;
}


checkItemExists* binarySearch(arrayOfStructs* array_of_str, dataNode* item, int first, int last)	
{

	checkItemExists* check = malloc(sizeof(checkItemExists));

	check->exists=false;
	

	dataNode* array = array_of_str->array;

    if (last < first){
		//printf("last<first\n");
    	check->exists=false;
		//printf("%s aaxxx %s arrays word\n",item->word,array[first].word);
		if(array[first].word!=NULL){
			if(strcmp(item->word,array[first].word)>0)
    			check->position=first + 1;
    		else
    			check->position=first;
		
		}else{
			check->position = first+1;
		
		}
    	
    	
    	return 	check;
    	
     }

 	int mid = (first+last)/2;
 
    if(strcmp(item->word ,array[mid].word)==0){
		check->position=mid;
		check->exists=true;
        return check;
     }
 
    if(strcmp(item->word,array[mid].word)>0)
        return binarySearch(array_of_str, item, mid+1, last);
    return binarySearch(array_of_str, item, first, mid-1);
}
 
 
 
// Function to sort an array_of_str->array  of size 'n'
checkItemExists* insertionSort(arrayOfStructs* array_of_str, dataNode* itemForInsert, int lastElement)
{
    int i, loc, j;
    
    checkItemExists* retPosition=malloc(sizeof(checkItemExists));
    retPosition->position=0;	//insert in first elements
    retPosition->exists=false;
    
	//printf("Last element is %d\n",lastElement);
	if(lastElement==0){
		array_of_str->array[0] = *itemForInsert;
		return retPosition;
	}
	i=lastElement;
    j = i - 1;

    // find location where selected sould be inseretd
    checkItemExists* getPosition = binarySearch(array_of_str, itemForInsert, 0, j);
    if(getPosition->exists==true){
    	//printf("word '%s' already exists in array in pos %d\n",itemForInsert->word,getPosition->position);
    	return getPosition;
    }

    loc=getPosition->position;
    // Move all elements after location to create space
    while (j >= loc)
    {
        array_of_str->array[j+1] = array_of_str->array[j];
        j--;
    }
    array_of_str->array[j+1] = *itemForInsert;
    getPosition->position=j+1;
    
    return getPosition;
}




void printArray(arrayOfStructs* array_of_str, int position){

	printf("ELEMENTS ARE: [");
	for(int k=0; k<= position;k++){

		printf("%s ",  array_of_str->array[k].word);
		
		if(k!=position)
			printf(", ");
	}
	printf("]\n");


}


//insert from query file
int executeQueryFile(FILE* file,arrayOfStructs* structureTree){

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
			insert = stringToArray(remainingLine,structureTree,'A');
			if (!insert){
				fprintf( stderr, "%s\n","Insert was unsuccessful");	
				break;
			}
		}
		else if(strcmp(wordCase,"Q")==0){
			printf("SEARCH\n");
			int search = stringToArray(remainingLine,structureTree,'Q');
			if (!search){
				fprintf( stderr, "%s\n","Search was unsuccessful");	
				break;
			}
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









