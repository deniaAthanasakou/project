#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"
#include "func.h"


void initializeArray(arrayOfStructs* array_of_str){
	array_of_str->position=0;
	array_of_str->length=10;
	array_of_str->array=malloc(array_of_str->length * sizeof(dataNode));
	array_of_str->array[array_of_str->position].word = NULL;
	for(int i=0;i<array_of_str->length;i++){
		array_of_str->array[i].nextWordArray = NULL;
	}
	
	
	
}


void doubleLength(arrayOfStructs* array_of_str){
	array_of_str->length*=2;
	array_of_str->array = (dataNode*) realloc(array_of_str->array , array_of_str->length * sizeof(dataNode));
	
}

/*void deleteArray(arrayOfStructs* array_of_str){		

	arrayOfStructs* tempArray = array_of_str;
	
	if(tempArray != NULL){
		//printf("pointer of tempArray= %p\n",tempArray);
		int lastElement = tempArray->position;
		printf("last element %d, length is %d\n",tempArray->position, tempArray->length);
	
		for(int i=0; i < lastElement; i++){
			//printf("before dlt i=%d with counter %d\n", i,counter);
			printf("Word is '%s'\n", tempArray->array[i].word);
			if( tempArray->array[i].nextWordArray!=NULL){
				printf("it wasn't null\n");
				deleteArray(tempArray->array[i].nextWordArray);
				printf("Delete array consisting of\n");
				printArray(tempArray,tempArray->position-1);
				
				
			}
			if(&(tempArray->array[i])==NULL)
				continue;
			
			deleteDataNode(&(tempArray->array[i]));
			tempArray->position--;
			printf("new position after delete is %d\n", tempArray->position);
			printf("after delete printing again\n");
			printArray(tempArray,tempArray->position-1);
		}
	}
	else
		printf("Null array_of_str\n");	
}*/




void deleteArray(arrayOfStructs* array_of_str){	

	arrayOfStructs* tempArray = array_of_str;
	
	if(tempArray != NULL){
		int lastElement = tempArray->position;
	
		for(int i=0; i < lastElement; i++){
			printf("'%s' is in position %d\n ", tempArray->array[i].word, i);
			
			if( tempArray->array[i].nextWordArray!=NULL){
				deleteArray( tempArray->array[i].nextWordArray);
				
			}
			else
				printf("found null array\n");
				
			printf("delete '%s'\n",tempArray->array[i].word);
			deleteDataNode(&(tempArray->array[i]));
			tempArray->position--;	
		}
		free(tempArray->array);
		tempArray->array=NULL;
		free(tempArray);
		tempArray=NULL;
	}
	else
		printf("Null array_of_str\n");	
		
}



void deleteDataNode(dataNode* elem){
	//printf("delete a\n");
	if( elem->word!=NULL){
		//printf("WORD IS NOT NYLL\n");
		free(elem->word);
		//printf("delete c\n");
		elem->word = NULL;
	}
	else
		//printf("WOWWWWW WORD IS NYLL\n");
	//printf("delete d pointer %p\n", elem);
	//free(elem);
	//printf("delete e\n");
	//elem = NULL;
	printf("delete done\n");
	
}
