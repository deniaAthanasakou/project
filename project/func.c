#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "func.h"
#include "stack.h"


int insert_ngram(arrayOfStructs* array_of_structs, char** arrayOfWords, int noOfWords){		//same layer

	arrayOfStructs* tempArray = array_of_structs;
	int i=0;
	
	for(i=0; i<noOfWords; i++){				//different layers
		
		int position = tempArray->position;
		
		dataNode* tempElement = malloc(sizeof(dataNode));
		if(i==noOfWords -1)		//final word
			tempElement->isFinal=true;
		else
			tempElement->isFinal=false;
			
		tempElement->word= (char*)malloc((strlen(arrayOfWords[i]) + 1) * sizeof(char));
		strcpy(tempElement->word,arrayOfWords[i]);	
		
		checkItemExists* getPosition = insertionSort(tempArray, tempElement, tempArray->position); 
		
		
		if(getPosition->exists==true){
			if(i==noOfWords -1)		//final word
				tempArray->array[getPosition->position].isFinal=true;
			//deleteDataNode(tempElement);
			
		}
		else{			//if word is not in array yet
		
			if(i==noOfWords -1)		//final word
				tempArray->array[getPosition->position].isFinal=true;
			else
				tempArray->array[getPosition->position].isFinal=false; 
				
			if(tempArray->position == tempArray->length-1){
				doubleLength(tempArray);
			}
			tempArray->position++;
			tempArray->array[getPosition->position].nextWordArray = malloc(1 * sizeof(arrayOfStructs));
			initializeArray(tempArray->array[getPosition->position].nextWordArray);

		}
		
		//deleteDataNode(tempElement);
		free(tempElement);
		tempElement=NULL;
		//printArray(tempArray, tempArray->position-1);
		tempArray = tempArray->array[getPosition->position].nextWordArray;
		free(getPosition);
		getPosition = NULL;
	}
	//printArray(array_of_structs, array_of_structs->position-1);

	return 1;
}

//search
void search_ngram(arrayOfStructs* array_of_structs, char** arrayOfWordsOriginal, int noOfWordsOriginal){		//is called for a single query
	//printf("<-------------------------SEARCH BEGINNING------------------------->\n");
	
	
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
		
			checkItemExists* getPosition = binarySearch(tempArray, tempElement, 0 ,tempArray->position); 
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
				}
				finalString = realloc(finalString,(strlen(finalString)+2)*sizeof(char));
				finalString[strlen(finalString)] = '\0';
				strcat(finalString, " ");
			}
			else{
				deleteDataNode(tempElement);
				free(getPosition);
				getPosition = NULL;
				break;
			}		
			tempArray = tempArray->array[getPosition->position].nextWordArray;
			if(tempArray == NULL)
			{
				deleteDataNode(tempElement);
				free(getPosition);
				getPosition = NULL;
				break;		
			}
			
			deleteDataNode(tempElement);
			free(getPosition);
			getPosition = NULL;
			
		}
		free(finalString);
		finalString = NULL;
		noOfWords--;
	}
	if(found==0)
		printf("-1\n");
	else	
		printf("\b ");				//remove last "|" 
	
	
	printf("\n");
	//printf("<-------------------------SEARCH ENDING------------------------->\n");
}

//delete
void delete_ngram(arrayOfStructs* array_of_structs, char** arrayOfWords, int noOfWords){
	//printf("<-------------------------DELETE STARTING------------------------->\n");
	arrayOfStructs* tempArray = array_of_structs;
	stack* myStack = malloc(sizeof(stack));
	initializeStack(myStack);
	for(int i=0; i<noOfWords; i++){
	
		//printArray(tempArray,tempArray->position-1);
		//printf("Must delete word '%s'\n",arrayOfWords[i]);
		int position = tempArray->position;
		
		dataNode* tempElement = malloc(sizeof(dataNode));
		tempElement->word= (char*)malloc((strlen(arrayOfWords[i])+1) * sizeof(char));
		strcpy(tempElement->word,arrayOfWords[i]);	
		
		
		//find out if word exists in array and if it does return position
		checkItemExists* getPosition = binarySearch(tempArray, tempElement,0 ,tempArray->position);
		if(	getPosition->exists==true){
			//printf("found inside array\n");
			push(myStack, getPosition->position);
		}
		else{										//element was not found inside array so it can not be deleted	
			//printf("element '%s' was not found \n",tempElement->word);
			deleteDataNode(tempElement);
			free(getPosition);
			getPosition = NULL;
			break;			
		}
			
		
		//printArrayFinalWords(tempArray,tempArray->position-1);
		
		tempArray = tempArray->array[getPosition->position].nextWordArray;
		if(tempArray == NULL)
		{
			deleteDataNode(tempElement);
			free(getPosition);
			getPosition = NULL;
			break;			
		}
		deleteDataNode(tempElement);
		free(getPosition);
		getPosition = NULL;
		
	}
	
	
	//displayStack(myStack);
	
	//delete elements in positions of myStack
	
	
	//while stack not empty
	while(!isEmpty(myStack)){
		//root of trie
		tempArray = array_of_structs;
		//displayStack(myStack);
		
		//get last element of stack
		for(int i=0;i <= myStack->top ;i++){
			if(tempArray != NULL){
				tempArray = tempArray->array[myStack->positionsToDelete[i]].nextWordArray;
			}
		}

		
		if(tempArray->array[myStack->positionsToDelete[myStack->top]].nextWordArray == NULL){			//hasnt got children
			deletionSort(tempArray,myStack->positionsToDelete[myStack->top], tempArray->position);
		}else{																						//has children
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
	
	deleteStack(myStack);
	free(myStack);
	myStack = NULL;
	
	//printf("<-------------------------DELETE ENDING------------------------->\n");



}
































