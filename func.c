#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "func.h"
#include "stack.h"
#include "bloomfilter.h"

void insert_ngram(arrayOfStructs* array_of_structs, HashTable* hashTable, char** arrayOfWords, int noOfWords){		//same layer
	arrayOfStructs* tempArray = array_of_structs;
	int i=0;

	
	for(i=0; i<noOfWords; i++){				//different layers
		int position = tempArray->position;
		
		
		
		dataNode* tempElement = malloc(sizeof(dataNode));		//creating dataNode for insert
		if(i==noOfWords -1)		//final word
			tempElement->isFinal=true;
		else
			tempElement->isFinal=false;
		
		insertString (tempElement, arrayOfWords[i]);
		tempElement->nextWordArray=NULL;
		
		insertTrieNode(tempElement, hashTable);					//inserting node into hashTable
		
		
		/*checkItemExists* getPosition = insertionSort(tempArray, tempElement, tempArray->position); 
		
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
		
		free(tempElement);
		tempElement=NULL;
		tempArray = tempArray->array[getPosition->position].nextWordArray;
		free(getPosition);
		getPosition = NULL;*/

	}
}

//search
char* search_ngram(arrayOfStructs* array_of_structs, char** arrayOfWordsOriginal, int noOfWordsOriginal){		//is called for a single query

	BloomFilter* filter = initializeFilter(5);		//initialize bloomFilter here
	
	char** finalStringArray=malloc(0*sizeof(char*));
	int itemsOffinalStringArray=0;
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
		
			insertString (tempElement, arrayOfWords[i]);
			//tempElement->word= (char*)malloc((strlen(arrayOfWords[i])+1) * sizeof(char));
			//strcpy(tempElement->word,arrayOfWords[i]);
		
			checkItemExists* getPosition = binarySearch(tempArray, tempElement, 0 ,tempArray->position,NULL); 
			if(getPosition->exists==true){		//if word was found
				strLength += strlen(arrayOfWords[i]) + 2;
				finalString = (char*)realloc(finalString,(strLength)*sizeof(char));
				
				if(i==0){
					char* word = getString(&(tempArray->array[getPosition->position]));
					strcpy(finalString, word);	
					free(word);
					word = NULL;					
				}
				else{
					finalString[strlen(finalString)] = '\0';
					char* word = getString(&(tempArray->array[getPosition->position]));
					strcat(finalString, word);	
					free(word);
					word = NULL;
				}

					
				if(tempArray->array[getPosition->position].isFinal == true){		//if true check whether finalString should be printed
					found=1;
					
					//if(!checkIfStringExists(finalStringArray,itemsOffinalStringArray, finalString)){		//finalString does not exist in finalStringArray
					if(!bloomFilterSeach(filter,finalString)){			//if finalString should be printed (is not in filter)
					//if(!bloomFilterSearch(finalString)){			//if finalString should be printed (is not in filter)
						addFilter(filter,finalString,strlen(finalString));		//add finalString in filter
						itemsOffinalStringArray++;
						finalStringArray=realloc(finalStringArray, itemsOffinalStringArray * sizeof(char*));
						finalStringArray[itemsOffinalStringArray-1]=malloc((strlen(finalString)+1)* sizeof(char));
						strcpy(finalStringArray[itemsOffinalStringArray-1], finalString);
					
						returningStringLength += strlen(finalString)+2;
						returningString=realloc(returningString,returningStringLength *sizeof(char));
						strcat(returningString,finalString);
						//returningString= realloc(returningString, (strlen(returningString)+1 +2)*sizeof(char));
						//strcat(returningString,"|");
					}
						
					
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
		printf("-1\n");
		returningString = realloc(returningString, 3*sizeof(char));
		strcpy(returningString,"-1");
	}
	else{	
		//printf("\b ");				//remove last "|" 
		//strcat(returningString,"\b");
		printQuery(finalStringArray, itemsOffinalStringArray);
	}	
	
	for(int i=0; i<itemsOffinalStringArray; i++){
		free(finalStringArray[i]);
		finalStringArray[i] = NULL;
	}
	free(finalStringArray);
	finalStringArray = NULL;
	
	
	freeFilter(filter);
	return returningString;
}

//delete
void delete_ngram(arrayOfStructs* array_of_structs, char** arrayOfWords, int noOfWords){
	//printf("Before\n");
	//printFullArray(array_of_structs,array_of_structs->position);
	arrayOfStructs* tempArray = array_of_structs;
	stack* myStack = malloc(sizeof(stack));
	initializeStack(myStack);
	for(int i=0; i<noOfWords; i++){

		int position = tempArray->position;
		
		dataNode* tempElement = malloc(sizeof(dataNode));
		insertString (tempElement, arrayOfWords[i]);
		//tempElement->word= (char*)malloc((strlen(arrayOfWords[i])+1) * sizeof(char));
		tempElement->nextWordArray=NULL;
		//strcpy(tempElement->word,arrayOfWords[i]);	
		
		
		//find out if word exists in array and if it does return position
		checkItemExists* getPosition = binarySearch(tempArray, tempElement,0 ,tempArray->position,NULL);
		if(	getPosition->exists==true){
			push(myStack, getPosition->position);
		}
		else{										//element was not found inside array so it can not be deleted	
			deleteDataNode(tempElement);
			free(tempElement);
			tempElement=NULL;
			free(getPosition);
			getPosition = NULL;
			
			deleteStack(myStack);
			free(myStack);
			myStack = NULL;
			
			return;	
		}
		
		tempArray = tempArray->array[getPosition->position].nextWordArray;
		if(tempArray == NULL)
		{
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

	
	int flagIfElementWasDeleted=0;
	int flagLastElementDeleted = 0;
	while(!isEmpty(myStack)){
		//root of trie
		tempArray = array_of_structs;
		
		//get last element of stack
		for(int i=0;i < myStack->top ;i++){
			if(tempArray != NULL){
				if(tempArray->array[myStack->positionsToDelete[i]].nextWordArray!=NULL){
					tempArray = tempArray->array[myStack->positionsToDelete[i]].nextWordArray;
				}
			}
		}

		if(flagIfElementWasDeleted==1 && tempArray->array[myStack->positionsToDelete[myStack->top]].nextWordArray->position==0) {
			deleteArray(tempArray->array[myStack->positionsToDelete[myStack->top]].nextWordArray);			
			tempArray->array[myStack->positionsToDelete[myStack->top]].nextWordArray=NULL;					
			
		}
		//if element not last && element.isFinal()
		
		if (flagLastElementDeleted ==1 && tempArray->array[myStack->positionsToDelete[myStack->top]].isFinal){
			break;
		}
		
		
		if(tempArray->array[myStack->positionsToDelete[myStack->top]].nextWordArray == NULL){			//hasnt got children
		//if(tempArray->array[myStack->positionsToDelete[myStack->top]].nextWordArray == NULL){			//hasnt got children
			//printf("worddd: %s\n",tempArray->array[myStack->positionsToDelete[myStack->top]].word);
			deletionSort(tempArray,myStack->positionsToDelete[myStack->top], tempArray->position);
			flagIfElementWasDeleted=1;
			flagLastElementDeleted = 1;
		}else{	
			tempArray->array[myStack->positionsToDelete[myStack->top]].isFinal = false;
			break;																			//final word -> not final and break
		}

		//pop top item
		int popElem = pop(myStack);
		if(!popElem){
			printf("Error with popping element from stack.\n");
			break;
		}
		
	}
	//printf("After\n");
	//printFullArray(array_of_structs,array_of_structs->position);
	
	deleteStack(myStack);
	free(myStack);
	myStack = NULL;
}
































