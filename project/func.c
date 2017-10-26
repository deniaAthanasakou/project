#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "func.h"


int insert_ngram(arrayOfStructs* array_of_structs, char** arrayOfWords, int noOfWords){		//same layer

	arrayOfStructs* tempArray = array_of_structs;	
	
	for(int i=0; i<noOfWords; i++){				//different layers
		
		int position = tempArray->position;
		int found = -1;
		if(position!=0){
			found = binarySearchSame(tempArray,arrayOfWords[i],position);
			printf("FOUND IS:%d\n",found);
		}
		
		if(found==-1){
			tempArray->array[position]= malloc(1 * sizeof(dataNode));
			tempArray->array[position]->word= (char*)malloc(strlen(arrayOfWords[i]) * sizeof(char));
			strcpy(tempArray->array[position]->word,arrayOfWords[i]);
			printf("word is %s\n",tempArray->array[position]->word);
		}		
		
		if(found!=-1){
			//found same word in array
			if(i==tempArray->length -1)		//final word
				tempArray->array[found]->isFinal=true;
			else{
				if(tempArray->array[found]->isFinal==false)		//if it wasnt true before 
					tempArray->array[found]->isFinal=false;
			}	
	
			tempArray = tempArray->array[found]->nextWordArray;
		}
		else{
			//not the same as others
			if(i==tempArray->length -1)		//final word
				tempArray->array[position]->isFinal=true;
			else
				tempArray->array[position]->isFinal=false; 
			
			printArray(tempArray, position);
			insertionSort(tempArray, tempArray->position);
			tempArray->position++;
		
			tempArray = tempArray->array[position]->nextWordArray;
			
		}
		tempArray =  (arrayOfStructs*) malloc(1 * sizeof(arrayOfStructs));
		initializeArray(tempArray);	
		tempArray->position=0;
	}
	
	insertionSort(array_of_structs, array_of_structs->position);
	printf("after insertion sort\n");
	printArray(array_of_structs, array_of_structs->position-1);

	return 1;
}

//delete

//search

