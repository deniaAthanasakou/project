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
			
		}
	}
}



int checkForCompression(dataNode* node){
	if(node->nextWordArray!=NULL){
		if (node->nextWordArray->position == 1){
			if(node->staticArray==NULL){
				node->staticArray = malloc(node->staticArrayLength *sizeof(int));
				char* startString = getString(node);
				if(node->isFinal){
					node->staticArray[node->staticArrayLength -1] = strlen(startString);
				}
				else{
					node->staticArray[node->staticArrayLength -1] = -strlen(startString);
				}
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
	
	startNode->noOfChars = startNode->noOfChars + additionalNode->noOfChars;
	char* newString = malloc((startNode->noOfChars+2)*sizeof(char));
	
	
	strcpy(newString, firstString);
	strcat(newString, secondString);
	
	
	startNode->staticArray = realloc(startNode->staticArray,(startNode->staticArrayLength +1) *sizeof(int));
	arrayOfStructs* tempArray = startNode->nextWordArray;
	
	startNode->nextWordArray = additionalNode->nextWordArray;
	startNode->word[0] = '\0';
	
	startNode->isDynamic = true;
	startNode->isFinal = additionalNode->isFinal;
	
	if(startNode->dynamicWord == NULL){
		startNode->dynamicWord = malloc(startNode->noOfChars * sizeof(char));
	}
	else{
		startNode->dynamicWord = realloc(startNode->dynamicWord,startNode->noOfChars * sizeof(char));
	}
	strcpy(startNode->dynamicWord,newString);
	
	
	if(additionalNode->isFinal)
		startNode->staticArray[startNode->staticArrayLength] = strlen(secondString);
	else
		startNode->staticArray[startNode->staticArrayLength] = -strlen(secondString);
	free(newString);
	newString = NULL;
	
	deleteArray1Layer(tempArray);

}



char** getNgramFromNode(dataNode* node){
	char** ngram=NULL;
	if(node->staticArray!=NULL){
		ngram = malloc((node->staticArrayLength+1)*sizeof(char*));
		int k=0;
		for(int i=0; i <= node->staticArrayLength; i++){
			int sizeMalloc = abs(node->staticArray[i]);
			ngram[i] = malloc(sizeof(char)*(sizeMalloc+1));
			int j = 0;
			for(j=0; j < sizeMalloc; j++){
				ngram[i][j] = node->dynamicWord[k];
				k++;
			}
			ngram[i][j] = '\0';
		}
	}
	else{		//1 word
		ngram = malloc(1*sizeof(char*));
		char* word = getString(node);
		ngram[0] = malloc((strlen(word) +1)*sizeof(char));
		strcpy(ngram[0], word);
	}
	return ngram;
}






