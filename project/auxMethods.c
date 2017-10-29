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
	if(query == 'D'){
		if(!delete_ngram(array, arrayOfWords,noOfWords)){
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
    	check->exists=false;
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

 	if(array[mid].word==NULL){			//to prosthesa egw dn kserw an einai swsto
 		check->exists=false;
 		check->position=-1;
		return check;
 	}
    if(strcmp(item->word ,array[mid].word)==0){
		check->position=mid;
		check->exists=true;
        return check;
     }
 
    if(strcmp(item->word,array[mid].word)>0){
        return binarySearch(array_of_str, item, mid+1, last);
        }
    return binarySearch(array_of_str, item, first, mid-1);
}
 
 
 
// Function to sort an array_of_str->array  of size 'n'
checkItemExists* insertionSort(arrayOfStructs* array_of_str, dataNode* itemForInsert, int lastElement)
{
    int i, loc, j;
    
    checkItemExists* retPosition=malloc(sizeof(checkItemExists));
    retPosition->position=0;	//insert in first elements
    retPosition->exists=false;
    
	if(lastElement==0){
		array_of_str->array[0] = *itemForInsert;
		return retPosition;
	}
	i=lastElement;
    j = i - 1;

    // find location where selected sould be inseretd
    checkItemExists* getPosition = binarySearch(array_of_str, itemForInsert, 0, j);
    if(getPosition->exists==true){
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

checkItemExists* deletionSort(arrayOfStructs* array_of_str, dataNode* itemForInsert, int lastElement){
	
    int i, loc, j;

	i=lastElement;
    j = i - 1;

    // find location where selected sould be inseretd
    checkItemExists* getPosition = binarySearch(array_of_str, itemForInsert, 0, j);
    if(getPosition->exists==true){
    
    	loc=getPosition->position;
		// Move all elements before location to create space
		while (loc < lastElement -1)
		{
		    array_of_str->array[loc] = array_of_str->array[loc+1];
		    loc++;
		}
		
		array_of_str->position--;
	}
     
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

void printArrayFinalWords(arrayOfStructs* array_of_str, int position){

	printf("FINAL ELEMENTS ARE: [");
	for(int k=0; k<= position;k++){
		if(array_of_str->array[k].isFinal){
			printf("%s ",  array_of_str->array[k].word);
		
			if(k!=position)
				printf(", ");
		}
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
			int search = stringToArray(remainingLine,structureTree,'Q');		//isws na nai void
			if (!search){
				fprintf( stderr, "%s\n","Search was unsuccessful");	
				break;
			}
		}
		else if(strcmp(wordCase,"D")==0){
			printf("DELETE\n");
			int delete = stringToArray(remainingLine,structureTree,'D');
			if (!delete){
				fprintf( stderr, "%s\n","Delete was unsuccessful");	
				break;
			}
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









