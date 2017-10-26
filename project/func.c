#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "func.h"


int insert_ngram(arrayOfStructs* array_of_structs, char** arrayOfWords, int noOfWords){		//same layer

	arrayOfStructs* tempArray = array_of_structs;
	int i=0;
	
	for(i=0; i<noOfWords; i++){				//different layers
	
		printf("POINTER IS AFTER %p\n",tempArray);
		printf("WORD IS %s\n", arrayOfWords[i]);
		
		int position = tempArray->position;
		
		dataNode* tempElement = malloc(sizeof(dataNode));
		if(i==noOfWords -1)		//final word
			tempElement->isFinal=true;
		else
			tempElement->isFinal=false;
			
		tempElement->word= (char*)malloc(strlen(arrayOfWords[i]) * sizeof(char));
		strcpy(tempElement->word,arrayOfWords[i]);	
		
		checkItemExists* getPosition = insertionSort(tempArray, tempElement, tempArray->position); 
		
		
		printf("After Insertion Sort Try\n");
		
		if(getPosition->exists==true){
			printf("FOUND INSIDE STRCMP BINARY3\n");
			if(i==noOfWords -1)		//final word
				tempArray->array[getPosition->position].isFinal=true;
			else{
				if(tempArray->array[getPosition->position].isFinal==false)		//if it wasnt true before 
					tempArray->array[getPosition->position].isFinal=false;
			}
			
			printArray(tempArray, getPosition->position);
			printf("inside if TRUE\n");
			tempArray = tempArray->array[getPosition->position].nextWordArray;
			//printf("POINTER IS %p\n",tempArray);
		
			
		}
		else{
		
			printf("inside else\n");
			
			printf("position is %d\n",getPosition->position);

		
			if(i==noOfWords -1)		//final word
				tempArray->array[getPosition->position].isFinal=true;
			else
				tempArray->array[getPosition->position].isFinal=false; 
				
			printf("after final\n");
			
			printArray(tempArray, getPosition->position);	
		
			tempArray->position++;
	
		}
		

		tempArray = tempArray->array[getPosition->position].nextWordArray;
		
		printf("before null\n");
		if(tempArray==NULL){
			printf("is null\n");
			tempArray = malloc(1 * sizeof(arrayOfStructs));
			//printf("POINTER IS %p\n",tempArray);
			initializeArray(tempArray);	
			tempArray->position=0;
		}
		
		printf("POINTER IS BEFORE %p\n",tempArray);
		
	}
	
	//insertionSort(array_of_structs, arrayOfWords[i-1],array_of_structs->position);
	printf("after insertion sort\n");
	printArray(array_of_structs, array_of_structs->position-1);

	return 1;
}

//delete

//search

