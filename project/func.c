#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "func.h"


int insert_ngram(arrayOfStructs* array_of_structs, char** arrayOfWords, int noOfWords){		//same layer

	arrayOfStructs* tempArray = array_of_structs;
	int i=0;
	
	for(i=0; i<noOfWords; i++){				//different layers
		//printf("I = %d\n",i);
		
		int position = tempArray->position;
		
		dataNode* tempElement = malloc(sizeof(dataNode));
		if(i==noOfWords -1)		//final word
			tempElement->isFinal=true;
		else
			tempElement->isFinal=false;
			
		tempElement->word= (char*)malloc(strlen(arrayOfWords[i]) * sizeof(char));
		strcpy(tempElement->word,arrayOfWords[i]);	
		
		checkItemExists* getPosition = insertionSort(tempArray, tempElement, tempArray->position); 
		
		if(getPosition->exists==true){
			if(i==noOfWords -1)		//final word
				tempArray->array[getPosition->position].isFinal=true;
		}
		else{			//if word is not in array yet
		
			if(i==noOfWords -1)		//final word
				tempArray->array[getPosition->position].isFinal=true;
			else
				tempArray->array[getPosition->position].isFinal=false; 
				
			if(tempArray->position == tempArray->length-1){
				doubleLength(tempArray);
			}
			tempArray->position++;
			//printf("WORD IS '%s' POINTER IS %p NEXT POSITION IS %d (SHOULD BE 1)\n ",arrayOfWords[i],tempArray,tempArray->position);
			tempArray->array[getPosition->position].nextWordArray = malloc(1 * sizeof(arrayOfStructs));
			initializeArray(tempArray->array[getPosition->position].nextWordArray);
			tempArray->array[getPosition->position].nextWordArray->position=0;
			

		}
		//printArray(tempArray, tempArray->position-1);
		tempArray = tempArray->array[getPosition->position].nextWordArray;
		
	}
	//printArray(array_of_structs, array_of_structs->position-1);

	return 1;
}

//search
int search_ngram(arrayOfStructs* array_of_structs, char** arrayOfWords, int noOfWords){

	arrayOfStructs* tempArray = array_of_structs;
	char* finalString;
	int i=0;
	int strLength=0;
	int found = 0;
	for(i=0; i < noOfWords; i++){				//different layers
		
		dataNode* tempElement = malloc(sizeof(dataNode));
		
		tempElement->word= (char*)malloc(strlen(arrayOfWords[i]) * sizeof(char));
		strcpy(tempElement->word,arrayOfWords[i]);
		
		checkItemExists* getPosition = binarySearch(tempArray, tempElement, 0 ,tempArray->position); 
		
		printf("AFTER BINARY\n");
		if(getPosition->exists==true){
			strLength += strlen(arrayOfWords[i]) + 1;
			finalString = realloc(finalString,strLength*sizeof(char));
			strcat(finalString,tempArray->array[getPosition->position].word);
			//printf("FINAL STRING IS:%s WITH ALLOCATED LENGTH:%d\n",finalString, strLength);
			if(tempArray->array[getPosition->position].isFinal == true){
				//printf("FINAL\n");
				found=1;
				//printf("FINAL STRING IS:%s WITH ALLOCATED LENGTH:%d\n",finalString, strLength);
				printf("%s|",finalString);
			}
			strcat(finalString, " ");
		}
		
		tempArray = tempArray->array[getPosition->position].nextWordArray;
		if(tempArray == NULL)
		{
			break;
		}
		
			
	}
	
	//not found 
	if(found == 0){
		return 0;
	}
	
	return 1;

}

//delete

