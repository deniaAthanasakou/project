#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"
#include "func.h"


void initializeArray(arrayOfStructs* array_of_str){
	array_of_str->position=0;
	array_of_str->length=10;
	array_of_str->array=malloc(array_of_str->length * sizeof(dataNode));
	for(int i=0;i<array_of_str->length;i++){
		initializeDataNode(&(array_of_str->array[i]));
	}
	
}

void doubleLength(arrayOfStructs* array_of_str){
	int lengthBefore=array_of_str->length;
	array_of_str->length*=2;
	array_of_str->array = (dataNode*) realloc(array_of_str->array , array_of_str->length * sizeof(dataNode));
	
	for(int i=lengthBefore;i<array_of_str->length;i++){
		initializeDataNode(&(array_of_str->array[i]));
	}
	
}

void initializeDataNode(dataNode* node){
	node->dynamicWord = NULL;
	node->nextWordArray = NULL;
	node->isDynamic=false;
	node->isFinal=false;
	node->noOfChars = -1;
	node->word[0]='\0';
	node->staticArray = NULL;
	node->staticArrayLength = 0;
	node->isEmpty = true;
}



void deleteArray(arrayOfStructs* array_of_str){	

	arrayOfStructs* tempArray = array_of_str;
	if(tempArray != NULL){
		if(tempArray->array==NULL){
			free(tempArray);
			tempArray=NULL;
			return;
		}
		int lastElement = tempArray->position;	
		for(int i=0; i < lastElement; i++){				
			if( &(tempArray->array[i])!=NULL){	
				deleteArray(tempArray->array[i].nextWordArray);
				if(tempArray->array[i].dynamicWord!=NULL){	
					deleteDataNode(&(tempArray->array[i]));
				}
			}
			else
				break;
		}
		free(tempArray->array);
		tempArray->array=NULL;
		free(tempArray);
		tempArray=NULL;
	}		
}

void deleteArray1Layer(arrayOfStructs* array_of_str){	

	if(array_of_str != NULL){
		if(array_of_str->array==NULL){
			free(array_of_str);
			array_of_str=NULL;
			return;
		}
		int lastElement = array_of_str->position;	
		for(int i=0; i < lastElement; i++){		
			deleteDataNode(&(array_of_str->array[i]));
			array_of_str->array[i].nextWordArray = NULL;
			
		}
		free(array_of_str->array);
		array_of_str->array=NULL;
		free(array_of_str);
		array_of_str=NULL;
	}		
}

void printStaticArray(dataNode* node){
	printf("Printing Static Array %d \n [ ",node->staticArrayLength+1);
	for(int i=0; i <= node->staticArrayLength; i++){
		if(i!=node->staticArrayLength)
			printf("%d, ",node->staticArray[i]);
		else 
			printf("%d ",node->staticArray[i]);
	}
	printf(" ]\n");

}


void deleteDataNode(dataNode* elem){
	if(elem!=NULL){
		if( elem->dynamicWord!=NULL){
			free(elem->dynamicWord);
			elem->dynamicWord = NULL;
		}
		if(elem->staticArray!=NULL){
			free(elem->staticArray);
			elem->staticArray = NULL;
		}
		//initializeDataNode(elem);
	}
}
