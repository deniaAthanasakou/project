#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compress.h"

void recreateStructure(HashTable* hashTable){
	for(int i=0;i<hashTable->length;i++){		//for each bucket
		for(int j=0; j<hashTable->buckets[i].position; j++){		//for each cell
			dataNode* cellNode = &hashTable->buckets[i].cells[j];		//get cell

			while(1){
				cellNode->staticArrayLength++;
				if(!checkForCompression(cellNode))
					break;

			}
			cellNode->staticArrayLength--;
			if(cellNode->nextWordArray !=NULL){
				recursiveCompression(cellNode->nextWordArray);
				
				//printFullArray(cellNode->nextWordArray,cellNode->nextWordArray->position);
				
			}
			
			
			
		}
	
	}

}




void recursiveCompression(arrayOfStructs* array){
	arrayOfStructs* tempArray = array;
	for(int i=0; i<tempArray->position; i++){		//for each element of array as startNode
		dataNode* startNode = &tempArray->array[i];
		
		while(1){
			startNode->staticArrayLength++;
			if(!checkForCompression(startNode))
				break;
		}
		startNode->staticArrayLength--;
		if(startNode->nextWordArray !=NULL){
			recursiveCompression(startNode->nextWordArray);
			
			//printFullArray(startNode->nextWordArray,startNode->nextWordArray->position);
			
		}
	}
}



int checkForCompression(dataNode* node){
	
	if(node->nextWordArray!=NULL){
		if (node->nextWordArray->position == 1){
			if(node->staticArray==NULL){
				node->staticArray = malloc(node->staticArrayLength *sizeof(int));
			}
			else{
				node->staticArray = realloc(node->staticArray, node->staticArrayLength *sizeof(int));
			}
			
			char* startString = getString(node);
			if(node->isFinal)
				node->staticArray[node->staticArrayLength -1] = strlen(startString);
			else
				node->staticArray[node->staticArrayLength -1] = -strlen(startString);
		
			free(startString);
			startString = NULL;
		
		
			compress(node,&node->nextWordArray->array[0]);
			return 1;
		}
	}
	return 0;
}



void compress(dataNode* startNode,dataNode* additionalNode){

	char *firstString = getString(startNode);
	char *secondString = getString(additionalNode);
	printf("firstString %s  secondString %s ",firstString,secondString);
	
	firstString = realloc(firstString,(strlen( firstString) + strlen( secondString) +2)*sizeof(char));
	strcat(firstString, secondString);
	
	
	startNode->staticArray = realloc(startNode->staticArray,(startNode->staticArrayLength +1) *sizeof(int));
	arrayOfStructs* tempArray = startNode->nextWordArray;
	
	printf("startNode->staticArrayLength %d\n",startNode->staticArrayLength);
	
	
	startNode->nextWordArray = additionalNode->nextWordArray;
	startNode->word[0] = '\0';
	startNode->isDynamic = true;
	startNode->isFinal = additionalNode->isFinal;
	startNode->noOfChars = startNode->noOfChars + additionalNode->noOfChars;
	if(startNode->dynamicWord == NULL){
		startNode->dynamicWord = malloc(startNode->noOfChars * sizeof(char));
		strcpy(startNode->dynamicWord,firstString);
	}
	else{
		startNode->dynamicWord = realloc(startNode->dynamicWord,startNode->noOfChars * sizeof(char));
		strcat(startNode->dynamicWord,secondString);
	}
	
	
	if(additionalNode->isFinal)
		startNode->staticArray[startNode->staticArrayLength] = strlen(secondString);
	else
		startNode->staticArray[startNode->staticArrayLength] = -strlen(secondString);	
	
	printf("newString %s \n",startNode->dynamicWord);
	
	//printf("Printing Static Array %d\n",startNode->staticArrayLength);
	printStaticArray(startNode);
	printf("\n\n");
	free(firstString);
	firstString = NULL;
	free(secondString);
	secondString = NULL;
	
	deleteArray1Layer(tempArray);

}

























