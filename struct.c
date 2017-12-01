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
	node->noOfChars = -1;
	node->word[0]='\0';
}



void deleteArray(arrayOfStructs* array_of_str){	

	arrayOfStructs* tempArray = array_of_str;
	if(tempArray != NULL){
	//printf("startdelete array\n");	
		//printf("begin with ptr of array:  %p\n",tempArray->array);	
		if(tempArray->array==NULL){
		//	printf("tempArray->array\n");	
			free(tempArray);
			tempArray=NULL;
			return;
		}
		//printf("after first if\n");
		int lastElement = tempArray->position;	
		//printf("lastElement %d\n",lastElement);	
		for(int i=0; i < lastElement; i++){
			//printf("for last %d i: %d\n", lastElement, i);
				
			if( &(tempArray->array[i])!=NULL){	
			//printf("a\n");
				//printf("noOfChars %d\n",tempArray->array[i].noOfChars);
		//	printf("b\n");
				deleteArray(tempArray->array[i].nextWordArray);
			//printf("afterif\n");	
				if(tempArray->array[i].dynamicWord!=NULL){	
			//		printf("ifif\n");				
					deleteDataNode(&(tempArray->array[i]));
				}
			}
			else
				break;
			
		}
		//printf("before free\n");
		free(tempArray->array);
		//printf("after free1\n");
		tempArray->array=NULL;
		free(tempArray);
	//	printf("after free2\n");
		tempArray=NULL;
	}	

		
}



void deleteDataNode(dataNode* elem){
//printf("deleteDataNode\n");
	if(elem!=NULL){
		//printf("word '%s'\n", elem->word);
		if( elem->dynamicWord!=NULL){
		//	printf("word '%s'\n", elem->dynamicWord);
			free(elem->dynamicWord);
			elem->dynamicWord = NULL;
		}
	}
}
