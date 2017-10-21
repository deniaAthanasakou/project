#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "func.h"


int insert_ngram(arrayOfStructs* array_of_structs, char** arrayOfWords, int noOfWords){

	arrayOfStructs* tempArray = array_of_structs;	
	
	for(int i=0; i<noOfWords; i++){
		
		int position = array_of_structs->position;
		
		int newPosition = binarySearch(position, arrayOfWords[i], tempArray);
		printf("new position is %d\n", newPosition);
		if(newPosition< position){	//move elements
			printf("NEWPOS < POSITION\n");
			tempArray->array[position]= malloc(1 * sizeof(dataNode));
			tempArray->array[position]->word= (char*)malloc(strlen(arrayOfWords[i]) * sizeof(char));
		
			for (int j=position-1; j>=newPosition; j--){
				//printf("j is %d\n", j);
				
				//printf("before move j=%d with %s \n", j, tempArray->array[j]->word);
				tempArray->array[j+1]=tempArray->array[j];
				printf("j=%d with %s and j+1=%d with %s\n", j, tempArray->array[j]->word,j+1,tempArray->array[j+1]->word);
			}
			printf("in if\n");
			printArray(tempArray, position);
			strcpy(tempArray->array[newPosition]->word,arrayOfWords[i]);
		
		}
		else{					//must allocate space for new word
			printf("COMPLE POSITION\n");
			tempArray->array[position]= malloc(1 * sizeof(dataNode));
			tempArray->array[position]->word= malloc(strlen(arrayOfWords[i]) * sizeof(char));
			strcpy(tempArray->array[position]->word,arrayOfWords[i]);
		}
		
		if(i==tempArray->length -1)		//final word
			tempArray->array[newPosition]->isFinal=true;
		else
			tempArray->array[newPosition]->isFinal=false; 
		
		printf("ending\n");
		printArray(tempArray, position);
		
		tempArray->position++;
			
		tempArray = tempArray->array[newPosition]->nextWordArray;
		tempArray =  (arrayOfStructs*) malloc(1 * sizeof(arrayOfStructs));
		initializeArray(tempArray);	
		tempArray->position=0;



		
		
	}
	

	return 1;
}

//delete

//search

