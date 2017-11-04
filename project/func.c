#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "func.h"
#include "stack.h"


void insert_ngram(arrayOfStructs* array_of_structs, char** arrayOfWords, int noOfWords){		//same layer
	//printf("STARTING INSERT	\n");
	arrayOfStructs* tempArray = array_of_structs;
	int i=0;
	
	//printf("noOfWords %d\n", noOfWords);
	//
	for(i=0; i<noOfWords; i++){				//different layers
		int position = tempArray->position;
		
		dataNode* tempElement = malloc(sizeof(dataNode));
		if(i==noOfWords -1)		//final word
			tempElement->isFinal=true;
		else
			tempElement->isFinal=false;
			
		tempElement->word= (char*)malloc((strlen(arrayOfWords[i]) + 1) * sizeof(char));
		tempElement->nextWordArray=NULL;
		strcpy(tempElement->word,arrayOfWords[i]);	
		
		checkItemExists* getPosition = insertionSort(tempArray, tempElement, tempArray->position); 
		
		if(getPosition->exists==true){
			if(i==noOfWords -1)		//final word
				tempArray->array[getPosition->position].isFinal=true;
			deleteDataNode(tempElement);
			
			
		}
		else{			//if word is not in array yet
			if(i==noOfWords -1)		//final word
				tempArray->array[getPosition->position].isFinal=true;
			else
				tempArray->array[getPosition->position].isFinal=false; 
				
			
			tempArray->position++;	
		}
		
		if(i!=noOfWords -1 && tempArray->array[getPosition->position].nextWordArray==NULL){
			tempArray->array[getPosition->position].nextWordArray = malloc(1 * sizeof(arrayOfStructs));
			initializeArray(tempArray->array[getPosition->position].nextWordArray);
		}
		
		//printf("pointer in insert '%s' is %p\n",arrayOfWords[i],tempArray->array[tempArray->position-1].word);
		
		//deleteDataNode(tempElement);
		free(tempElement);
		tempElement=NULL;
		//printArray(tempArray, tempArray->position-1);
		tempArray = tempArray->array[getPosition->position].nextWordArray;
		free(getPosition);
		getPosition = NULL;
		
		
		
	}
	
	
	//printf("pointer2 in insert '%s' is %p\n",tempArray->array[1].word,tempArray->array[1].word);
	//printArray(array_of_structs, array_of_structs->position-1);
	//printf("ENDING INSERT\n	\n\n");
}

//search
char* search_ngram(arrayOfStructs* array_of_structs, char** arrayOfWordsOriginal, int noOfWordsOriginal){		//is called for a single query
	//printf("<-------------------------SEARCH BEGINNING------------------------->\n");
	//printf("\n\n");
	
	char* returningString=malloc(1*sizeof(char));
	strcpy(returningString,"");			//initialization
	int returningStringLength=0;
	int found = 0;
	int noOfWords=noOfWordsOriginal;
	for(int j=0; j < noOfWordsOriginal; j++){	//for each word of query starting as first Word
		
		arrayOfStructs* tempArray = array_of_structs;
		char* finalString= NULL;
		int strLength=0;

		char* arrayOfWords[noOfWords];
		int counter=j;
		for(int k=0; k<noOfWords; k++){
			arrayOfWords[k]=arrayOfWordsOriginal[counter];
			counter++;
		}
	
		for(int i=0; i < noOfWords; i++){				//for each word of query
		
			dataNode* tempElement = malloc(sizeof(dataNode));
		
			tempElement->word= (char*)malloc((strlen(arrayOfWords[i])+1) * sizeof(char));
			strcpy(tempElement->word,arrayOfWords[i]);
		
			checkItemExists* getPosition = binarySearch(tempArray, tempElement, 0 ,tempArray->position,NULL); 
			if(getPosition->exists==true){
				strLength += strlen(arrayOfWords[i]) + 2;
				finalString = (char*)realloc(finalString,(strLength)*sizeof(char));
				
				if(i==0){
					strcpy(finalString, tempArray->array[getPosition->position].word);						
				}
				else{
					finalString[strlen(finalString)] = '\0';
					strcat(finalString,tempArray->array[getPosition->position].word);
				}
					
				//printf("FINAL STRING IS:%s WITH ALLOCATED LENGTH:%d\n",finalString, strLength);
				if(tempArray->array[getPosition->position].isFinal == true){
					found=1;
					//printf("FINAL STRING  WITH ALLOCATED LENGTH %d IS : %s|\n", strLength, finalString);
					printf("%s|", finalString);
					
					
					returningStringLength += strlen(finalString)+2;
					returningString=realloc(returningString,returningStringLength *sizeof(char));
					strcat(returningString,finalString);
					returningString= realloc(returningString, (strlen(returningString)+1 +2)*sizeof(char));
					strcat(returningString,"|");
					
		
					
					
					
				}
				finalString = realloc(finalString,(strlen(finalString)+2)*sizeof(char));
				finalString[strlen(finalString)] = '\0';
				strcat(finalString, " ");
			}
			else{
				deleteDataNode(tempElement);
				free(tempElement);
				tempElement=NULL;
				free(getPosition);
				getPosition = NULL;
				break;
			}		
			tempArray = tempArray->array[getPosition->position].nextWordArray;
			if(tempArray == NULL)
			{
				deleteDataNode(tempElement);
				free(tempElement);
				tempElement=NULL;
				free(getPosition);
				getPosition = NULL;
				break;		
			}
			
			deleteDataNode(tempElement);
			free(tempElement);
			tempElement=NULL;
			free(getPosition);
			getPosition = NULL;
			
		}
		
		free(finalString);
		finalString = NULL;
		noOfWords--;
	}
	if(found==0){
		printf("-1");
		returningString = realloc(returningString, 3*sizeof(char));
		strcpy(returningString,"-1");
	}
	else{	
		printf("\b ");				//remove last "|" 
		strcat(returningString,"\b");
	}
	
	
	
	printf("\n");
	
	return returningString;
	//printf("<-------------------------SEARCH ENDING------------------------->\n");
}

//delete
void delete_ngram(arrayOfStructs* array_of_structs, char** arrayOfWords, int noOfWords){
	//printf("<-------------------------DELETE STARTING------------------------->\n");
	
	
	//printf("\n\n\nPRINTING FULL TREE before delete \n\n\n");
	
	//printFullArray(array_of_structs,array_of_structs->position);
	
	//printf("\n\n noOfWords %d\n",noOfWords);
	
	arrayOfStructs* tempArray = array_of_structs;
	stack* myStack = malloc(sizeof(stack));
	initializeStack(myStack);
	for(int i=0; i<noOfWords; i++){
	
		//printf("i=%d\n",i);
	
		//printArray(tempArray,tempArray->position-1);
		//printf("Must delete word '%s'\n",arrayOfWords[i]);
		int position = tempArray->position;
		
		dataNode* tempElement = malloc(sizeof(dataNode));
		tempElement->word= (char*)malloc((strlen(arrayOfWords[i])+1) * sizeof(char));
		tempElement->nextWordArray=NULL;
		strcpy(tempElement->word,arrayOfWords[i]);	
		
		
		//find out if word exists in array and if it does return position
		checkItemExists* getPosition = binarySearch(tempArray, tempElement,0 ,tempArray->position,NULL);
		if(	getPosition->exists==true){
			//printf("element '%s' found inside array\n",tempElement->word);
			push(myStack, getPosition->position);
		}
		else{										//element was not found inside array so it can not be deleted	
			//printf("element '%s' was not found \n",tempElement->word);
			deleteDataNode(tempElement);
			free(tempElement);
			tempElement=NULL;
			free(getPosition);
			getPosition = NULL;
			
			deleteStack(myStack);
			free(myStack);
			myStack = NULL;
			
			return;			//changed			
			//free stuff
		}
			
		
		//printArrayFinalWords(tempArray,tempArray->position-1);
		
		tempArray = tempArray->array[getPosition->position].nextWordArray;
		if(tempArray == NULL)
		{
		//printf("AXXXXXXXXXXXX\n");
			deleteDataNode(tempElement);
			free(tempElement);
			tempElement=NULL;
			free(getPosition);
			getPosition = NULL;
			if(i!=noOfWords-1){
				deleteStack(myStack);
				free(myStack);
				myStack = NULL;
				return;	
			}
			break;			
		}
		deleteDataNode(tempElement);
		free(tempElement);
		tempElement=NULL;
		free(getPosition);
		getPosition = NULL;
		
	}
	
	
	//displayStack(myStack);
	
	//delete elements in positions of myStack
	
	int flagIfElementWasDeleted=0;
	//while stack not empty
	while(!isEmpty(myStack)){
		//printf("WHILE\n");
		//root of trie
		tempArray = array_of_structs;
		//displayStack(myStack);
		
		//get last element of stack
		for(int i=0;i < myStack->top ;i++){
			if(tempArray != NULL){
				if(tempArray->array[myStack->positionsToDelete[i]].nextWordArray!=NULL){
					tempArray = tempArray->array[myStack->positionsToDelete[i]].nextWordArray;
					//printf("word is %s\n",tempArray->array[0].word);
				}
			}
		}
	///	printf("after for\n");
		//printf("word is %s\n",tempArray->array[0].word);
		//
		if(flagIfElementWasDeleted==1 && tempArray->array[myStack->positionsToDelete[myStack->top]].nextWordArray->position==0) {
			//deleteArray
			deleteArray(tempArray->array[myStack->positionsToDelete[myStack->top]].nextWordArray);			//-> auto vgazei sega	//telika mallon dn evgaze
			tempArray->array[myStack->positionsToDelete[myStack->top]].nextWordArray=NULL;					//-> auto vgazei sega	//telika mallon dn evgaze
			
		}
		
		
		
		if(tempArray->array[myStack->positionsToDelete[myStack->top]].nextWordArray == NULL){			//hasnt got children
		//printf("no children pos %d\n",tempArray->position);
			deletionSort(tempArray,myStack->positionsToDelete[myStack->top], tempArray->position);
			flagIfElementWasDeleted=1;
			//tempArray->array[myStack->positionsToDelete[myStack->top]].nextWordArray = NULL;
		}else{	
		//printf("children\n");																					//has children
			tempArray->array[myStack->positionsToDelete[myStack->top]].isFinal = false;
			break;																			//final word -> not final and exit
		}

		//pop top item
		int popElem = pop(myStack);
		if(!popElem){
			printf("Error with popping element from stack.\n");
			break;
		}
		
	}
	
	
//	printf("\n\n\nPRINTING FULL TREE after delete \n\n\n");
//	
//printFullArray(array_of_structs,array_of_structs->position);
	
	
	//printf("\n\n\nPRINTING FINAL after delete \n\n\n");
	
	//printArrayFinalWords(array_of_structs,array_of_structs->position-1);
	
	deleteStack(myStack);
	free(myStack);
	myStack = NULL;
//	
	//printf("\n\n<-------------------------DELETE ENDING------------------------->\n");
//


}
































