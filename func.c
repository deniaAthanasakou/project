#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "func.h"
#include "stack.h"
#include "bloomfilter.h"
#include "compress.h"

void insert_ngram(HashTable* hashTable, char** arrayOfWords, int noOfWords){		//same layer
	arrayOfStructs* tempArray =NULL ;
	int i=0;
	dataNode* insertedElement = NULL;
	
	for(i=0; i<noOfWords; i++){				//different layers
		if(i==0){			//root word
			dataNode* rootElement=NULL;
			rootElement = (dataNode*)malloc(1*sizeof(dataNode));		//creating dataNode for insert
			initializeDataNode(rootElement);
			if(i==noOfWords -1)		//final word
				rootElement->isFinal=true;
			else
				rootElement->isFinal=false;

			insertString (rootElement, arrayOfWords[i]);
			insertedElement = insertTrieNode(rootElement, hashTable);					//inserting node into hashTable

			deleteDataNode(rootElement);
			free(rootElement);
			rootElement=NULL;	
		}
		else{
			

			dataNode* tempElement=NULL;
			tempElement = (dataNode*)malloc(1*sizeof(dataNode));		//creating dataNode for insert
			initializeDataNode(tempElement);
			if(i==noOfWords -1)		//final word
				tempElement->isFinal=true;
			else
				tempElement->isFinal=false;

			insertString (tempElement, arrayOfWords[i]);
						
			tempElement->nextWordArray=NULL;
			if(i==1){
				if(insertedElement->nextWordArray==NULL){
					insertedElement->nextWordArray =  malloc(1 * sizeof(arrayOfStructs));
					initializeArray(insertedElement->nextWordArray);
				}
				tempArray = insertedElement->nextWordArray;	
					
			}
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
				if(i==noOfWords-1){
					tempArray->array[getPosition->position].nextWordArray=NULL;
				}
			}
			if(i!=noOfWords -1 && tempArray->array[getPosition->position].nextWordArray==NULL){
				tempArray->array[getPosition->position].nextWordArray = malloc(1 * sizeof(arrayOfStructs));
				initializeArray(tempArray->array[getPosition->position].nextWordArray);

			}
			free(tempElement);
			tempElement=NULL;
			tempArray = tempArray->array[getPosition->position].nextWordArray;
			free(getPosition);
			getPosition = NULL;
		
		}
	}
}

//search
char* search_ngram(HashTable *hashTable, char** arrayOfWordsOriginal, int noOfWordsOriginal, BloomFilter* topFilter,topKArray *topArray){		//is called for a single query

	BloomFilter* filter = initializeFilter(5);		//initialize bloomFilter here
	
	char** finalStringArray=malloc(0*sizeof(char*));
	int itemsOffinalStringArray=0;
	char* returningString=malloc(1*sizeof(char));
	strcpy(returningString,"");			//initialization
	int returningStringLength=0;
	int found = 0;
	int noOfWords=noOfWordsOriginal;
	for(int j=0; j < noOfWordsOriginal; j++){	//for each word of query starting as first Word
		
		dataNode* firstElement;
		arrayOfStructs *tempArray;
		
		char* finalString= NULL;
		int strLength=0;

		char* arrayOfWords[noOfWords];
		int counter=j;
		for(int k=0; k<noOfWords; k++){
			arrayOfWords[k]=arrayOfWordsOriginal[counter];
			counter++;
		}
		for(int i=0; i < noOfWords; i++){				//for each word of query
			if(i==0){
				firstElement = lookupTrieNode(arrayOfWords[i],hashTable);
				if(firstElement==NULL){
					break;
				}
				strLength += strlen(arrayOfWords[i]) + 2;
				finalString = (char*)realloc(finalString,(strLength)*sizeof(char));
				strcpy(finalString,arrayOfWords[i]);
				
				if(firstElement->isFinal){
					found=1;
					if(!possiblyContains(filter,finalString,strlen(finalString))){			//if finalString should be printed (is not in filter)
						addFilter(filter,finalString,strlen(finalString));		//add finalString in filter
						itemsOffinalStringArray++;
						finalStringArray=realloc(finalStringArray, itemsOffinalStringArray * sizeof(char*));
						finalStringArray[itemsOffinalStringArray-1]=malloc((strlen(finalString)+1)* sizeof(char));
						strcpy(finalStringArray[itemsOffinalStringArray-1], finalString);

						//insert to topArray
						if(!possiblyContains(topFilter,finalString,strlen(finalString))){		//if finalString does not exist in array
							addFilter(topFilter,finalString,strlen(finalString));
							if(topArray->positionInsertion == topArray->length){
								doubleTopKArray(topArray);
							}
							insertTopArray(topArray,finalString);
						}
						else{																	//exists in array
							//printf("final Strind %s\n", finalString);
							//printFullArrayTop(topArray);
							binarySearchTopK(topArray->array, finalString, topArray->positionInsertion);
						}
						HeapSort(topArray->array, topArray->positionInsertion, 1);	//sort based on strings
						returningStringLength += strlen(finalString)+2;
						returningString=realloc(returningString,returningStringLength *sizeof(char));
						strcat(returningString,finalString);
						
					}
				}
				finalString = realloc(finalString,(strlen(finalString)+2)*sizeof(char));
				finalString[strlen(finalString)] = '\0';
				strcat(finalString, " ");
			}
			else{
				if(i==1){
					tempArray = firstElement->nextWordArray;
					if(tempArray == NULL)
					{
						break;		
					}

				}
				dataNode* tempElement = malloc(sizeof(dataNode));
				insertString (tempElement, arrayOfWords[i]);
				checkItemExists* getPosition = binarySearch(tempArray, tempElement, 0 ,tempArray->position,NULL); 
				if(getPosition->exists==true){		//if word was found
					strLength += strlen(arrayOfWords[i]) + 2;
					finalString = (char*)realloc(finalString,(strLength)*sizeof(char));
				
					finalString[strlen(finalString)] = '\0';
					char* word = getString(&(tempArray->array[getPosition->position]));
					strcat(finalString, word);	
					free(word);
					word = NULL;
					
					if(tempArray->array[getPosition->position].isFinal == true){		//if true check whether finalString should be printed
						found=1;
						
						//finalString does not exist in finalStringArray
						if(!possiblyContains(filter,finalString,strlen(finalString))){			//if finalString should be printed (is not in filter)
							addFilter(filter,finalString,strlen(finalString));		//add finalString in filter
							itemsOffinalStringArray++;
							finalStringArray=realloc(finalStringArray, itemsOffinalStringArray * sizeof(char*));
							finalStringArray[itemsOffinalStringArray-1]=malloc((strlen(finalString)+1)* sizeof(char));
							strcpy(finalStringArray[itemsOffinalStringArray-1], finalString);
							
							//insert to topArray
							if(!possiblyContains(topFilter,finalString,strlen(finalString))){
								addFilter(topFilter,finalString,strlen(finalString));
								if(topArray->positionInsertion == topArray->length){
									doubleTopKArray(topArray);
								}
								insertTopArray(topArray,finalString);
							}
							else{		//exists in array
								//printf("final Strind %s\n", finalString);
								//printFullArrayTop(topArray);
								binarySearchTopK(topArray->array, finalString, topArray->positionInsertion);
							
							}
							//HeapSort(topArray->array, topArray->positionInsertion);
							HeapSort(topArray->array, topArray->positionInsertion, 1);	//sort based on strings
							returningStringLength += strlen(finalString)+2;
							returningString=realloc(returningString,returningStringLength *sizeof(char));
							strcat(returningString,finalString);
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


//search in static files
char* search_ngram_StaticVersion(HashTable *hashTable, char** arrayOfWordsOriginal, int noOfWordsOriginal, BloomFilter* topFilter, topKArray *topArray){
	BloomFilter* filter = initializeFilter(5);		//initialize bloomFilter here
	
	char** finalStringArray=malloc(0*sizeof(char*));
	int itemsOffinalStringArray=0;
	char* returningString=malloc(1*sizeof(char));
	strcpy(returningString,"");			//initialization
	int returningStringLength=0;
	int found = 0;
	int noOfWords=noOfWordsOriginal;
	for(int j=0; j < noOfWordsOriginal; j++){	//for each word of query starting as first Word
		//printf("First Word %s\n",arrayOfWordsOriginal[j]);
		dataNode* tempElement = NULL;
		arrayOfStructs *tempArray = NULL;
		
		char* finalString= NULL;
		int strLength=0;
		int flagFound = 0;

		char* arrayOfWords[noOfWords];
		int counter=j;
		for(int k=0; k<noOfWords; k++){
			arrayOfWords[k]=arrayOfWordsOriginal[counter];
			counter++;
		}
		char** convertedStrings = NULL;
		//printf("before inner for\n");
		for(int i=0; i < noOfWords; i++){				//for each word of query
			//printf("word inner %s\n",arrayOfWords[i]);
			if(i==0){
				//printf("before lookup\n");
				tempElement = lookupTrieNode(arrayOfWords[i],hashTable);
				if (tempElement == NULL)
				{	
					//printf("Element %s not found inside hashTable\n",arrayOfWords[i]);
					break;
				}
				flagFound = 1;
				//printf("Element %s found inside hashTable\n",arrayOfWords[i]);
				
				strLength += strlen(arrayOfWords[i]) + 2;
				finalString = (char*)realloc(finalString,(strLength)*sizeof(char));
				strcpy(finalString,arrayOfWords[i]);
				
			}
			else{
				strLength += strlen(arrayOfWords[i]) + 2;
				finalString = (char*)realloc(finalString,(strLength)*sizeof(char));
				strcat(finalString,arrayOfWords[i]);
			}
			
			
			//printf("finalString '%s'\n", finalString);
			convertedStrings = getNgramFromNode(tempElement);
			//printf("after getNgramFromNode\n");
			int counterForArray = 0;
			if(tempElement->staticArray!=NULL){					//if tempElement is supernode
				//printf("------------SUPERRRRRRRRRRRR\n");
				while(counterForArray <= tempElement->staticArrayLength){
					//printf("-------------INSIDE WHILE convertedStrings[counterForArray] = %s,arrayOfWords[i] = %s \n",convertedStrings[counterForArray],arrayOfWords[i]);
					
					if(strcmp(convertedStrings[counterForArray],arrayOfWords[i])==0){  //if found
						if(tempElement->staticArray[counterForArray]>0){				//if final
							found=1;
							if(counterForArray!=0){
								strLength += strlen(arrayOfWords[i]) + 2;
								finalString = (char*)realloc(finalString,(strLength)*sizeof(char));
			
								finalString[strlen(finalString)] = '\0';
								//char* word = getString(&(tempArray->array[getPosition->position]));
								strcat(finalString, convertedStrings[counterForArray]);	
								//free(word);
								//word = NULL;
							}
							
							
							if(!possiblyContains(filter,finalString,strlen(finalString))){			//if finalString should be printed (is not in filter)
								addFilter(filter,finalString,strlen(finalString));		//add finalString in filter
								itemsOffinalStringArray++;
								finalStringArray=realloc(finalStringArray, itemsOffinalStringArray * sizeof(char*));
								finalStringArray[itemsOffinalStringArray-1]=malloc((strlen(finalString)+1)* sizeof(char));
								strcpy(finalStringArray[itemsOffinalStringArray-1], finalString);

								//insert to topArray
								if(!possiblyContains(topFilter,finalString,strlen(finalString))){		//if finalString does not exist in array
									addFilter(topFilter,finalString,strlen(finalString));
									if(topArray->positionInsertion == topArray->length){
										doubleTopKArray(topArray);
									}
									insertTopArray(topArray,finalString);
								}
								else{																	//exists in array
									//printf("final Strind %s\n", finalString);
									//printFullArrayTop(topArray);
									binarySearchTopK(topArray->array, finalString, topArray->positionInsertion);
								}
								HeapSort(topArray->array, topArray->positionInsertion, 1);	//sort based on strings
								returningStringLength += strlen(finalString)+2;
								returningString=realloc(returningString,returningStringLength *sizeof(char));
								strcat(returningString,finalString);
				
							}	
						}
						i++;
						finalString = realloc(finalString,(strlen(finalString)+2)*sizeof(char));
						finalString[strlen(finalString)] = '\0';
						strcat(finalString, " ");
						//printf("finalString %s\n",finalString);
						//printf("returningString %s\n",returningString);
					}
					else{
						//maybe free
						break;
					}	
					counterForArray++;
				}
			}
			else {					// if tempElement is not a supernode
				//printf("not a supernode\n");
				if(tempElement->isFinal){
					found=1;
						
					/*strLength += strlen(arrayOfWords[i]) + 2;
					finalString = (char*)realloc(finalString,(strLength)*sizeof(char));
	
					finalString[strlen(finalString)] = '\0';
					//char* word = getString(&(tempArray->array[getPosition->position]));
					strcat(finalString, convertedStrings[counterForArray]);	
					//free(word);
					//word = NULL;*/
					
					
					if(!possiblyContains(filter,finalString,strlen(finalString))){			//if finalString should be printed (is not in filter)
						addFilter(filter,finalString,strlen(finalString));		//add finalString in filter
						itemsOffinalStringArray++;
						finalStringArray=realloc(finalStringArray, itemsOffinalStringArray * sizeof(char*));
						finalStringArray[itemsOffinalStringArray-1]=malloc((strlen(finalString)+1)* sizeof(char));
						strcpy(finalStringArray[itemsOffinalStringArray-1], finalString);

						//insert to topArray
						if(!possiblyContains(topFilter,finalString,strlen(finalString))){		//if finalString does not exist in array
							addFilter(topFilter,finalString,strlen(finalString));
							if(topArray->positionInsertion == topArray->length){
								doubleTopKArray(topArray);
							}
							insertTopArray(topArray,finalString);
						}
						else{																	//exists in array
							//printf("final Strind %s\n", finalString);
							//printFullArrayTop(topArray);
							binarySearchTopK(topArray->array, finalString, topArray->positionInsertion);
						}
						HeapSort(topArray->array, topArray->positionInsertion, 1);	//sort based on strings
						returningStringLength += strlen(finalString)+2;
						returningString=realloc(returningString,returningStringLength *sizeof(char));
						strcat(returningString,finalString);
		
					}	
					//i++;
				
				}
				//printf("after is final\n");
				finalString = realloc(finalString,(strlen(finalString)+2)*sizeof(char));
				finalString[strlen(finalString)] = '\0';
				strcat(finalString, " ");
			//	printf("finalString '%s'\n",finalString);
				//printf("returningString %s\n",returningString);
			}
			
			
			
			
			
			//printf("before tempArray\n");
			//get next tempElement
			tempArray = tempElement->nextWordArray;
			if(tempArray==NULL){
				//printf("iss null\n");
				break;
			}
			
			int position=i;
			if(flagFound)
				 position = i+1;
				 
			dataNode* madeUpElement = malloc(sizeof(dataNode));
			insertString (madeUpElement, arrayOfWords[position]);
			//printf("before binary '%s' I = %d\n",getString(madeUpElement), position);
			checkItemExists* getPosition = binarySearch(tempArray, madeUpElement, 0 ,tempArray->position,NULL); 
			//printf("after binary\n");
			if(getPosition->exists){
				tempElement = getPosition->insertedNode;
				//printf("ELEMENT IS %s\n", getString(tempElement));
			}
			else{
			//	printf("ELSE\n");
				break;
			}
			
			
		}
		//printf("AFTER INNER FOR\n");	
			
	}	
	
	
//	printf("AFTER ALL\n");
	if(found==0){
		printf("-1\n");
		returningString = realloc(returningString, 3*sizeof(char));
		strcpy(returningString,"-1");
	}
	else{	
		printQuery(finalStringArray, itemsOffinalStringArray);
	}	
	return returningString;	
				
}

//delete
void delete_ngram(HashTable* hashTable, char** arrayOfWords, int noOfWords){
	arrayOfStructs* tempArray = NULL;
	stack* myStack = malloc(sizeof(stack));
	initializeStack(myStack);
	dataNode* lookupElement=NULL;
	bool doNotDelete = false;
	for(int i=0; i<noOfWords; i++){
	
	
		if(i==0){
			lookupElement = lookupTrieNode(arrayOfWords[i] ,hashTable);
			if(lookupElement==NULL){
				deleteStack(myStack);
				free(myStack);
				myStack = NULL;
				return;
			}
			
			if(lookupElement->nextWordArray!=NULL){
				doNotDelete = true;
			}
			
			if(i==noOfWords-1){
				lookupElement->isFinal=false;
			}
		}
		else{
			if(i==1){
				tempArray = lookupElement->nextWordArray;
			}

			int position = tempArray->position;
		
			dataNode* tempElement = malloc(sizeof(dataNode));
			insertString (tempElement, arrayOfWords[i]);
			tempElement->nextWordArray=NULL;
			
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
		
	}
	
	
	int flagIfElementWasDeleted=0;
	int flagLastElementDeleted = 0;
	while(!isEmpty(myStack)){
		//root of trie
		tempArray = lookupElement->nextWordArray;
		
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
			doNotDelete = true;
			break;
		}
		
		
		if(tempArray->array[myStack->positionsToDelete[myStack->top]].nextWordArray == NULL){			//hasnt got children
			deletionSort(tempArray,myStack->positionsToDelete[myStack->top], tempArray->position);
			flagIfElementWasDeleted=1;
			flagLastElementDeleted = 1;
		}else{	
			tempArray->array[myStack->positionsToDelete[myStack->top]].isFinal = false;
			doNotDelete = true;
			break;																			//final word -> not final and break
		}

		//pop top item
		int popElem = pop(myStack);
		if(!popElem){
			printf("Error with popping element from stack.\n");
			break;
		}
		
	}
	
	if(!doNotDelete){
		char* lookUpWord = getString(lookupElement);
		int bucket = getBucketFromHash(hashTable->level, hashTable->initialLength, hashTable->bucketToBeSplit, lookUpWord);	
		int cell = getCell(lookUpWord , hashTable,bucket);
		deletionSortBucket(&hashTable->buckets[bucket], cell);
		free(lookUpWord);
		lookUpWord = NULL;
	}
	
	
	deleteStack(myStack);
	free(myStack);
	myStack = NULL;
	
}


































