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
	array_of_str->length*=2;
	array_of_str->array = (dataNode*) realloc(array_of_str->array , array_of_str->length * sizeof(dataNode));
	
}



void deleteArray(arrayOfStructs* array_of_str){	

	//printf("h\n");
	arrayOfStructs* tempArray = array_of_str;
	
	if(tempArray != NULL){
		//printf("b\n");
		int lastElement = tempArray->length;
		//printf("lastElement = %d\n",lastElement);
	
		for(int i=0; i < lastElement; i++){
		//	printf("'%s' is in position %d\n ", tempArray->array[i].word, i);
			//printf("c\n");
			if( tempArray->array[i].nextWordArray!=NULL){
			//printf("d\n");
				deleteArray( tempArray->array[i].nextWordArray);
				//printf("i\n");
				
			}
			//else
			//	printf("gg\n");
			//printf("e\n");
			//else
			//	printf("found null array\n");
				
			//printf("delete '%s'\n",tempArray->array[i].word);
			deleteDataNode(&(tempArray->array[i]));
			tempArray->position--;	
		}
		//printf("f\n");
		free(tempArray->array);
		tempArray->array=NULL;
		free(tempArray);
		tempArray=NULL;
	}
	//printf("g\n");
	//else
		//printf("Null array_of_str\n");	
		
}



void deleteDataNode(dataNode* elem){
	//printf("delete a\n");
	if(elem!=NULL){
		if( elem->word!=NULL){
			free(elem->word);
			elem->word = NULL;
		}
	}
}
