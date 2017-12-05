#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
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
				char* startString = getString(node);
				if(node->isFinal)
					node->staticArray[node->staticArrayLength -1] = strlen(startString);
				else
					node->staticArray[node->staticArrayLength -1] = -strlen(startString);
		
				free(startString);
				startString = NULL;
			}
			else{
				node->staticArray = realloc(node->staticArray, node->staticArrayLength *sizeof(int));
			}
			
			
		
		
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
	
	printf("startNode->staticArrayLength %d\n",startNode->staticArrayLength +1);
	
	
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
	
	printf("startNode->staticArrayLength %d\n",startNode->staticArrayLength);
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

char** getNgramFromNode(dataNode* node){
	//printf("InitialString %s\n",getString(node));
	char** ngram;
	if(node->staticArray!=NULL){
		ngram = malloc(node->staticArrayLength*sizeof(char*));
		int k=0;
		char* temp;
		for(int i=0; i <= node->staticArrayLength; i++){
			int sizeMalloc = abs(node->staticArray[i]);
			//printf("sizeMalloc is '%d'\n",sizeMalloc);
			ngram[i] = malloc(sizeof(char)*(sizeMalloc));
			int j = 0;
			for(j=0; j< sizeMalloc; j++){
				ngram[i][j] = node->dynamicWord[k];
				k++;
			}
			ngram[i][j] = '\0';
			//printf("%dth ngram is '%s'\n",i,ngram[i]);
			
		}
	}
	else{		//1 word
		//printf("WHAT\n");
		ngram = malloc(1*sizeof(char*));
		char* word = getString(node);
		ngram[0] = malloc((strlen(word) +1)*sizeof(char));
		strcpy(ngram[0], word);
		free(word);
		word = NULL;
	}
	return ngram;
}






















