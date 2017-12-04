#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compress.h"

void recreateStructure(HashTable* hashTable){
	for(int i=0;i<hashTable->length;i++){		//for each bucket
		//dataNode* temp = hashTable->buckets[i];
		for(int j=0; j<hashTable->buckets[i].position; j++){		//for each cell
			dataNode* temp = &hashTable->buckets[i].cells[j];		//get cell
			temp->staticArray = malloc(1*sizeof(int));				
			temp->staticArrayLength = 1;
			char* tempString = getString(temp);
			printf("tempString %s\n", tempString);
			if(temp->isFinal)
				temp->staticArray[0] = strlen(tempString);
			else
				temp->staticArray[0] = -strlen(tempString);
			
			free(tempString);
			tempString = NULL;
			arrayOfStructs *sameLayer = temp->nextWordArray;
			arrayOfStructs *nextArray = temp->nextWordArray;			//nextWordArray of bucket
			for(int k=0; k <sameLayer->position; k++){					//for each dataNode of nextWordArray->breadth first search
				printf("k = %d\n", k);
		
				while(nextArray!=NULL){									//until nextWordArray is null->depth-first-search
					dataNode *element = &nextArray->array[k];			//get kth item of nextWordArray
					printf("element %s\n",element->word);
					if(nextArray->position!=1){	//must not be compressed		//if not alone
						printf("pos1\n");
						nextArray = element->nextWordArray;						//go to nextWordArray of that item
						temp = element;										//get first element
						continue;
					}
				
					temp->staticArrayLength++;								//else compress
					compress(temp,element);
					nextArray = element->nextWordArray;						//nextWordArray is of kth element
				
				}
				
			}
		}
	
	}

}
/*procedure DFS(G,v):
     label v as discovered
     for all edges from v to w in G.adjacentEdges(v) do 
         if vertex w is not labeled as discovered then
            recursively call DFS(G,w)*/

void compress(dataNode* startNode,dataNode* additionalNode){

	char *firstString = getString(startNode);
	char *secondString = getString(additionalNode);
	printf("firstString %s  secondString %s\n",firstString,secondString);
	
	firstString = realloc(firstString,(strlen( firstString) + strlen( secondString) +2)*sizeof(char));
	strcat(firstString, secondString);
	
	
	startNode->staticArray = realloc(startNode->staticArray,startNode->staticArrayLength *sizeof(int));
	startNode->nextWordArray = additionalNode->nextWordArray;
	startNode->word[0] = '\0';
	startNode->isDynamic = true;
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
		startNode->staticArray[startNode->staticArrayLength -1] = strlen(secondString);
	else
		startNode->staticArray[startNode->staticArrayLength -1] = -strlen(secondString);	
	
	printf("newString %s \n",startNode->dynamicWord);
	
	printf("Printing Static Array\n");
	for(int i=0; i <startNode->staticArrayLength; i++){
		printf("%d ",startNode->staticArray[i]); 
	}
	printf("\n");
	
	free(firstString);
	firstString = NULL;
	free(secondString);
	secondString = NULL;

}

























