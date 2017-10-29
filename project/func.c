#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "func.h"


int insert_ngram(arrayOfStructs* array_of_structs, char** arrayOfWords, int noOfWords){		//same layer

	arrayOfStructs* tempArray = array_of_structs;
	//printf("LENGTH IS %d\n",tempArray->length);
	int i=0;
	
	for(i=0; i<noOfWords; i++){				//different layers
		
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
			tempArray->array[getPosition->position].nextWordArray = malloc(1 * sizeof(arrayOfStructs));
			initializeArray(tempArray->array[getPosition->position].nextWordArray);
			//tempArray->array[getPosition->position].nextWordArray->position=0;		//to evgala giati ginetai mesa sto initialize
			

		}
		printArray(tempArray, tempArray->position-1);
		tempArray = tempArray->array[getPosition->position].nextWordArray;
		
	}
	printArray(array_of_structs, array_of_structs->position-1);

	return 1;
}

//search
int search_ngram(arrayOfStructs* array_of_structs, char** arrayOfWordsOriginal, int noOfWordsOriginal){		//is called for a single query
	printf("<-------------------------SEARCH BEGINNING------------------------->\n");
	
	
	int found = 0;
	int noOfWords=noOfWordsOriginal;
	for(int j=0; j < noOfWordsOriginal; j++){	//for each word of query starting as first Word
		
		arrayOfStructs* tempArray = array_of_structs;
		char* finalString=malloc(sizeof(char));
		int strLength=0;

		char* arrayOfWords[noOfWords];
		int counter=j;
		//printf("Creating arrayOfWords: ");
		for(int k=0; k<noOfWords; k++){
			arrayOfWords[k]=arrayOfWordsOriginal[counter];
			counter++;
			//printf(" %s ", arrayOfWords[k]);
		}
		//printf("\n");
	
		for(int i=0; i < noOfWords; i++){				//for each word of query
		//	printf("WOW word of array is %s\n",arrayOfWords[i]);
		
			dataNode* tempElement = malloc(sizeof(dataNode));
		
			tempElement->word= (char*)malloc(strlen(arrayOfWords[i]) * sizeof(char));
			strcpy(tempElement->word,arrayOfWords[i]);
		
			checkItemExists* getPosition = binarySearch(tempArray, tempElement, 0 ,tempArray->position); 
			//printf("After binary\n");
			if(getPosition->exists==true){
				strLength += strlen(arrayOfWords[i]) + 1;
				finalString = realloc(finalString,strLength*sizeof(char));
				strcat(finalString,tempArray->array[getPosition->position].word);
				//printf("FINAL STRING IS:%s WITH ALLOCATED LENGTH:%d\n",finalString, strLength);
				if(tempArray->array[getPosition->position].isFinal == true){
					found=1;
					//printf("FINAL STRING  WITH ALLOCATED LENGTH %d IS : %s|\n", strLength, finalString);
					printf("%s|", finalString);
				}
				strcat(finalString, " ");
			}
			else
				break;
		
			tempArray = tempArray->array[getPosition->position].nextWordArray;
			if(tempArray == NULL)
			{
				break;			//isws return 1
			}
			
		}
		noOfWords--;
	}
	
	//not found 
	/*if(found == 0){			//auto giati xreiazetai?
		return 0;
	}*/
	printf("\n");
	printf("<-------------------------SEARCH ENDING------------------------->\n");
	
	return 1;

}

//delete

