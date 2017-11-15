#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "auxMethods.h"
#include "struct.h"


void initialize(FILE* file, arrayOfStructs* structureTree){

	char *line = NULL;
	size_t len = 0;
	int read;

	if (file == NULL)
		return;
	while ((read = getline(&line, &len, file)) != -1) {	
		char* ngram = strtok(line, "\n");
		callBasicFuncs(ngram,structureTree,'A');
	}
	//found eof

	if (line){
		free(line);
		line=NULL;
	}
}


void callBasicFuncs(char* ngram, arrayOfStructs* array, char query){

	arrayWords* arrayW = stringToArray(ngram);
	int noOfWords = arrayW->length;
	char** arrayOfWords = arrayW->words;
	if(query == 'A'){
		insert_ngram(array, arrayOfWords,noOfWords);
	}
	else if(query == 'Q'){
		char* searchString = search_ngram(array, arrayOfWords,noOfWords);
		free(searchString);
		searchString=NULL;
	}
	else if(query == 'D'){
		delete_ngram(array, arrayOfWords,noOfWords);
	}
	
	deleteArrayOfWords(arrayOfWords,noOfWords);
	if(arrayOfWords!=NULL){
		free(arrayOfWords);
		arrayOfWords=NULL;
		free(arrayW);
		arrayW = NULL;
	}
	
}

int checkIfStringExists(char** array, int noOfWords, char* str){
	for(int i=0; i<noOfWords; i++){
		if(strcmp(str,array[i])==0)
			return 1;
	}
	return 0;
}



arrayWords* stringToArray(char* ngram){
	char* pch;
	char** arrayOfWords; 
	pch = strtok (ngram," \t");
	int noOfWords=0;
	arrayOfWords = malloc(noOfWords * sizeof(char*));
	while (pch != NULL)
	{	
		noOfWords++;
		arrayOfWords = (char**)realloc(arrayOfWords, noOfWords * sizeof(char*));		
		arrayOfWords[noOfWords-1]=malloc((strlen(pch)+1)* sizeof(char));
		strcpy(arrayOfWords[noOfWords-1],pch);    //add pch into arrayOfWords
		pch = strtok (NULL, " \t");
	}
	
	arrayWords* arrayW = malloc(sizeof(arrayWords));
	arrayW->length = noOfWords;
	arrayW->words = arrayOfWords;
	
	return arrayW;

}

void deleteArrayOfWords(char** array,int length){
	if(array!=NULL){
		for(int i=0;i<length;i++){
			if( array[i]!=NULL){
				free(array[i]);
				array[i] = NULL;
			}
		}
		
	}

}

checkItemExists* binarySearch(arrayOfStructs* array_of_str, dataNode* item, int first, int last, checkItemExists* check)	
{
	if(check==NULL)
		check = malloc(sizeof(checkItemExists));

	check->exists=false;
	
	dataNode* array = array_of_str->array;
    if (last < first){
    	check->exists=false;
		if(array[first].word!=NULL){
			if(strcmp(item->word,array[first].word)>0){
    			check->position=first + 1;
    		}
    		else{
    			check->position=first;
    		}
		
		}else{
			check->position = first+1;
		}
    	return 	check;
    	
     }

 	int mid = (first+last)/2;

 	if(array[mid].word==NULL){			
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
        return binarySearch(array_of_str, item, mid+1, last, check);
    }
    return binarySearch(array_of_str, item, first, mid-1, check);
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
	free(retPosition);
	retPosition=NULL;
	i=lastElement;
    j = i - 1;

    // find location where selected sould be inseretd
    checkItemExists* getPosition = binarySearch(array_of_str, itemForInsert, 0, j,NULL);
    if(getPosition->exists==true){
    	return getPosition;
    }

	//double array
	if(array_of_str->position == array_of_str->length-1){
		doubleLength(array_of_str);
	}

    loc=getPosition->position;
    
    // Move all elements after location to create space
    while (j >= loc)
    {
        array_of_str->array[j+1] = array_of_str->array[j];
    
        j--;
    }
	/*printf("before + %d with loc=%d and j=%d\n",loc-j-1,loc,j);
	if(j>=loc){
		memmove(&array_of_str->array[j+1], &array_of_str->array[j], (loc-j-1)*sizeof(array_of_str->array));
	}
    printf("after\n");*/
	array_of_str->array[j+1] = *itemForInsert;
    getPosition->position=j+1;
    
    return getPosition;
}

void deletionSort(arrayOfStructs* array_of_str,	int position, int lastElement){
	
	if(array_of_str->array[position].word!=NULL){		
		free(array_of_str->array[position].word);
		array_of_str->array[position].word=NULL;
	}
	if(array_of_str->array[position].nextWordArray!=NULL){
		deleteArray(array_of_str->array[position].nextWordArray);
		array_of_str->array[position].nextWordArray=NULL;
	}
	

	while (position < lastElement -1)
	{
	    array_of_str->array[position]=array_of_str->array[position+1];
	    position++;
	}

	array_of_str->position--;
     
}

void printFullArray(arrayOfStructs* array_of_str, int position){	//prints all layers
	arrayOfStructs* tempArray = array_of_str;
	
	if(tempArray != NULL){
		int lastElement = tempArray->position;
	
		for(int i=0; i < lastElement; i++){
			printf("'%s' is in position %d, ", tempArray->array[i].word, i);
			if(i==lastElement-1){				//print only once
				printf("\b\b: ");
				printArray(tempArray,(tempArray->position-1));
			}
			if( tempArray->array[i].nextWordArray!=NULL){
				printFullArray( tempArray->array[i].nextWordArray, tempArray->array[i].nextWordArray->position);
				
			}
		}
	}
		
}

void printArray(arrayOfStructs* array_of_str, int position){		//prints layer

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
int executeQueryFile(FILE* file ,arrayOfStructs* structureTree){

	char *line = NULL;
	size_t len = 0;
	int read;

	if (file == NULL)
		return 1;
		
	char startingLetter = 'F';			//initialization			

	while ((read = getline(&line, &len, file)) != -1) {
	
		char* ngram = strtok(line, "\n");
		
		//find first letter for each case
		char* wordCase = strtok(ngram," \t");
		char* remainingLine = strtok(NULL,"");

		if(strcmp(wordCase,"A")==0){	
			startingLetter = 'A';
			callBasicFuncs(remainingLine,structureTree,'A');
		}
		else if(strcmp(wordCase,"Q")==0){
			startingLetter = 'Q';
			callBasicFuncs(remainingLine,structureTree,'Q');		
		}
		else if(strcmp(wordCase,"D")==0){
			startingLetter = 'D';
			callBasicFuncs(remainingLine,structureTree,'D');
		}
		else if(strcmp(wordCase,"F")==0){
			
			startingLetter = 'F';
		}
		else{			//different letter
			deleteArray(structureTree);
			structureTree=NULL;
			if (line){
				free(line);
				line=NULL;
			}
			return 0;
		}
		
			
	}//found eof
	
	
	deleteArray(structureTree);
	structureTree=NULL;
	if (line){
		free(line);
		line=NULL;
	}
	
	if(startingLetter !='F'){
		return 0;
	}
	

	return 1;

}

void printQuery(char** items, int iterNum){
	
	for(int i=0;i<iterNum;i++){
		if(i==iterNum-1){
			printf("%s\n",items[i]);
		}
		else{
			printf("%s|",items[i]);
		}		
	}

}









