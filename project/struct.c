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
		array_of_str->array[i].word = NULL;
		array_of_str->array[i].nextWordArray = NULL;
	}
	
}

void doubleLength(arrayOfStructs* array_of_str){
	int lengthBefore=array_of_str->length;
	array_of_str->length*=2;
	array_of_str->array = (dataNode*) realloc(array_of_str->array , array_of_str->length * sizeof(dataNode));
	
	for(int i=lengthBefore;i<array_of_str->length;i++){
		array_of_str->array[i].word = NULL;
		array_of_str->array[i].nextWordArray = NULL;
	}
	
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
		//printf("hsgfgs  pointer %p\n",tempArray->array);
		//printf("lastElement = %d\n",lastElement);
	
		for(int i=0; i < lastElement; i++){
			//printf("gsajgd\n");
			//printf("POINTER dsadas array %p\n",tempArray->array);
			//printf("POINTER array %p\n",&(tempArray->array[i]));
		//printf("'%s' is in position %d\n ", tempArray->array[i].word, i);
			if( &(tempArray->array[i])!=NULL){
				deleteArray(tempArray->array[i].nextWordArray);
				if(tempArray->array[i].word!=NULL)				//isws prepei na vgei
					deleteDataNode(&(tempArray->array[i]));
			}
			else
				break;
			
			//tempArray->position--;	
		}
		//printf("POINTER temparray %p\n",tempArray);
		
		free(tempArray->array);
		tempArray->array=NULL;
		free(tempArray);
		tempArray=NULL;
	}	
		
}



void deleteDataNode(dataNode* elem){
	//printf("delete a\n");
	if(elem!=NULL){
		if( elem->word!=NULL){
			//printf("delete word '%s'\n", elem->word);
			//printf("pointer in delete data node is %p\n",elem->word);
			free(elem->word);
			//printf("b\n");
			elem->word = NULL;
		}
		//else
			//printf("NULLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL\n");
	}
}
