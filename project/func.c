#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "func.h"


int insert_ngram(arrayOfStructs* array_of_structs, char** arrayOfWords, int noOfWords){		//same layer

	arrayOfStructs* tempArray = array_of_structs;
	int i=0;
	
	for(i=0; i<noOfWords; i++){				//different layers
		printf("I = %d\n",i);
		/*if(!strcmp(arrayOfWords[i],"a")){
			printf("THIS IS THE WORD WE'VE BEEN LOOKING FOR \"A\"\n");
			printf("position is %d (should be 1)\n",tempArray->position);
			printf("POINTER IS %p\n",tempArray);
		}*/
		
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
			else{																									//AUTO TO ELSE EINAI LATHOS
				if(tempArray->array[getPosition->position].isFinal==false)		//if it wasnt true before 
					tempArray->array[getPosition->position].isFinal=false;
			}
			printf("word inside array already is '%s'\n",tempArray->array[getPosition->position].word);
			
			//printArray(tempArray, getPosition->position);
			printf("inside if TRUE\n");
			if(!strcmp(arrayOfWords[i],"this"))
				printf("POINTER OF 'THIS' IS %p POSITION IS %d\n",tempArray,getPosition->position);
				printf("POINTER FOR NEXT WORD OF 'THIS' IS %p\n",tempArray->array[getPosition->position].nextWordArray);
			printf("hhaha\n");
			

		
			
		}
		else{			//if word is not in array yet
		
			if(i==noOfWords -1)		//final word
				tempArray->array[getPosition->position].isFinal=true;
			else
				tempArray->array[getPosition->position].isFinal=false; 
			
			//printArray(tempArray, getPosition->position);	
		
			tempArray->position++;
			printf("WORD IS '%s' POINTER IS %p NEXT POSITION IS %d (SHOULD BE 1)\n ",arrayOfWords[i],tempArray,tempArray->position);

		}
		

		tempArray = tempArray->array[getPosition->position].nextWordArray;
		
		//printf("before null\n");
		if(tempArray==NULL){
			tempArray = malloc(1 * sizeof(arrayOfStructs));
			if(!strcmp(arrayOfWords[i],"this"))
				printf("WORD IS '%s' and next word POINTER IS %p\n",arrayOfWords[i],tempArray);
			initializeArray(tempArray);	
			tempArray->position=0;
		}
		else
			printf("NOT NULL NEXT WORD\n");
		
		
	}
	printArray(array_of_structs, array_of_structs->position-1);

	return 1;
}

//delete

//search

