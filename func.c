#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "func.h"
#include "stack.h"
#include "bloomfilter.h"
#include "compress.h"

void insert_ngram(HashTable* hashTable, char** arrayOfWords, int noOfWords, int additionNum){		//same layer
	arrayOfStructs* tempArray =NULL ;
	int i=0;
	dataNode* insertedElement = NULL;
	
	for(i=0; i<noOfWords; i++){				//different layers
		if(i==0){			//root word
			dataNode* rootElement=NULL;
			rootElement = (dataNode*)malloc(1*sizeof(dataNode));		//creating dataNode for insert
			initializeDataNode(rootElement);
			rootElement->additionVersion = additionNum;
			if(i==noOfWords -1){		//final word
				rootElement->isFinal=true;
				rootElement->finalSinceAdditionVersion = additionNum;
			}
			else
				rootElement->isFinal=false;

			insertString (rootElement, arrayOfWords[i]);
			insertedElement = insertTrieNode(rootElement, hashTable);					//inserting node into hashTable

			free(rootElement);
			rootElement=NULL;	
			
			
		}
		else{


			dataNode* tempElement=NULL;
			tempElement = (dataNode*)malloc(1*sizeof(dataNode));		//creating dataNode for insert
			initializeDataNode(tempElement);
			tempElement->additionVersion = additionNum;
			if(i==noOfWords -1){		//final word
				tempElement->isFinal=true;
				tempElement->finalSinceAdditionVersion = additionNum;
			}
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
				if(i==noOfWords -1){		//final word
					tempArray->array[getPosition->position].isFinal=true;
					tempArray->array[getPosition->position].finalSinceAdditionVersion = additionNum;
				}
				deleteDataNode(tempElement);
			
			}
			else{			//if word is not in array yet
				if(i==noOfWords -1){		//final word
					tempArray->array[getPosition->position].isFinal=true;
					tempArray->array[getPosition->position].finalSinceAdditionVersion = additionNum;
				}
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
char* search_ngram(void** args){		//is called for a single query

	HashTable *hashTable = (HashTable*) args[0];
	char** arrayOfWordsOriginal = (char**) args[1];
	int noOfWordsOriginal = (*(int*)args[2]);
	BloomFilter* topFilter = (BloomFilter*)args[3];
	topKArray *topArray = (topKArray*)args[4];
	int queryNum = (*(int*)args[5]);
	
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
		char **arrayOfWords = arrayOfWordsOriginal;
		int counter = 0;
		for(int i=j; i < noOfWordsOriginal; i++){				//for each word of query
			if(counter==0){
				firstElement = lookupTrieNode(arrayOfWords[i],hashTable);
				if(firstElement==NULL){
					break;
				}
				
				strLength += strlen(arrayOfWords[i]) + 2;
				finalString = (char*)realloc(finalString,(strLength)*sizeof(char));
				strcpy(finalString,arrayOfWords[i]);

				if(firstElement->isFinal && queryNum>=firstElement->additionVersion && (queryNum>=firstElement->finalSinceAdditionVersion && firstElement->finalSinceAdditionVersion!=-1)&&(queryNum<firstElement->deletionVersion || firstElement->deletionVersion==-1) && (queryNum <firstElement->notFinalInDeletionVersion || firstElement->notFinalInDeletionVersion==-1)){
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
						else{		
							int itemPos = binarySearchTopK(topArray->array, finalString,0, topArray->positionInsertion-1);
							topArray->array[itemPos].occurences++;
						}
						
						returningStringLength += strlen(finalString)+2;
						returningString=realloc(returningString,returningStringLength *sizeof(char));
						strcat(returningString,finalString);
					}
				}
				finalString = realloc(finalString,(strlen(finalString)+2)*sizeof(char));
				finalString[strlen(finalString)] = '\0';
				strcat(finalString, " ");
			}				
			else{			//i!=j
				if(counter==1){
					tempArray = firstElement->nextWordArray;
					
					if(tempArray == NULL)
					{	
						break;		
					}

				}
				dataNode* tempElement = malloc(sizeof(dataNode));
				tempElement->staticArray = NULL;
				tempElement->staticArrayLength = 0;
				insertString (tempElement, arrayOfWords[i]);
				checkItemExists* getPosition = binarySearch(tempArray, tempElement, 0 ,tempArray->position-1,NULL, false); 

				if(getPosition->exists==true){		//if word was found
	
					strLength += strlen(arrayOfWords[i]) + 2;
					finalString = (char*)realloc(finalString,(strLength)*sizeof(char));
					finalString[strlen(finalString)] = '\0';
					char* word = getString(&(tempArray->array[getPosition->position]));
					strcat(finalString, word);	
				
				
					if(tempArray->array[getPosition->position].isFinal == true && queryNum>=tempArray->array[getPosition->position].additionVersion && (queryNum>=tempArray->array[getPosition->position].finalSinceAdditionVersion && queryNum>=tempArray->array[getPosition->position].finalSinceAdditionVersion!=-1) && (queryNum<tempArray->array[getPosition->position].deletionVersion || tempArray->array[getPosition->position].deletionVersion==-1 && (queryNum<tempArray->array[getPosition->position].notFinalInDeletionVersion || tempArray->array[getPosition->position].notFinalInDeletionVersion==-1))){		//if true check whether finalString should be printed
						found=1;
						
						//finalString does not exist in finalStringArray
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
							else{		
								int itemPos = binarySearchTopK(topArray->array, finalString,0, topArray->positionInsertion-1);
								topArray->array[itemPos].occurences++;
							}
							
							returningStringLength += strlen(finalString)+2;
							returningString=realloc(returningString,returningStringLength *sizeof(char));
							strcat(returningString,finalString);
						}

					}
					finalString = realloc(finalString,(strlen(finalString)+2)*sizeof(char));
					finalString[strlen(finalString)] = '\0';
					strcat(finalString, " ");
				}
				else{		//if element does not exist
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
			counter++;
		}
		free(finalString);
		finalString = NULL;
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
char* search_ngram_StaticVersion(void** args){
	
	HashTable *hashTable = (HashTable*) args[0];
	char** arrayOfWordsOriginal = (char**) args[1];
	BloomFilter* topFilter = (BloomFilter*)args[3];
	topKArray *topArray = (topKArray*)args[4];
	int noOfWordsOriginal = (*(int*)args[2]);


	BloomFilter* filter = initializeFilter(5);		//initialize bloomFilter here
	char** finalStringArray=malloc(0*sizeof(char*));
	int itemsOffinalStringArray=0;
	char* returningString=malloc(1*sizeof(char));
	strcpy(returningString,"");			//initialization
	int returningStringLength=0;
	int found = 0;
	int noOfWords=noOfWordsOriginal;
	for(int j=0; j < noOfWordsOriginal; j++){	//for each word of query starting as first Word
		
		dataNode* tempElement = NULL;
		arrayOfStructs *tempArray = NULL;
		
		char* finalString= NULL;
		int strLength=0;
		int flagFound = 0;
		int flagCounterIncreased = 0;
		int subFlag = 0;
		int flagFinalString = 0;
		int flagStringWasNotFound = 0;
		
		char** arrayOfWords = arrayOfWordsOriginal;
		char** convertedStrings = NULL;
		int counter = 0;
		
		for(int i=j; i < noOfWordsOriginal; i++){				//for each word of query
			flagStringWasNotFound = 0;
			flagCounterIncreased = 0;
			
			
			if(counter==0){
				tempElement = lookupTrieNode(arrayOfWords[i],hashTable);
				if (tempElement == NULL)
				{	
					break;	
				}
				flagFound = 1;
				
				strLength += strlen(arrayOfWords[i]) + 2;
				finalString = (char*)realloc(finalString,(strLength)*sizeof(char));
				strcpy(finalString,arrayOfWords[i]);
				
			}
			else if(subFlag==0 || tempElement->staticArray==NULL){
				strLength += strlen(arrayOfWords[i]) + 2;
				finalString = (char*)realloc(finalString,(strLength)*sizeof(char));
				strcat(finalString,arrayOfWords[i]);
				flagFinalString = 1;
			}
			
			convertedStrings = getNgramFromNode(tempElement);
			int counterForArray = 0;
			if(tempElement->staticArray!=NULL){					//if tempElement is supernode
				
				while(counterForArray <= tempElement->staticArrayLength){
					flagCounterIncreased=0;
					if(strcmp(convertedStrings[counterForArray],arrayOfWords[i])==0){  //if found
						if(counter!=0 && flagFinalString != 1){
							strLength += strlen(arrayOfWords[i]) + 2;
							finalString = (char*)realloc(finalString,(strLength)*sizeof(char));

							finalString[strlen(finalString)] = '\0';
							strcat(finalString, arrayOfWords[i]);	
						}
						if(tempElement->staticArray[counterForArray]>0){				//if final
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
								else{		
									int itemPos = binarySearchTopK(topArray->array, finalString,0, topArray->positionInsertion-1);
									topArray->array[itemPos].occurences++;	
								}
								
								returningStringLength += strlen(finalString)+2;
								returningString=realloc(returningString,returningStringLength *sizeof(char));
								strcat(returningString,finalString);
				
							}	
						}
						
						finalString = realloc(finalString,(strlen(finalString)+2)*sizeof(char));
						finalString[strlen(finalString)] = '\0';
						strcat(finalString, " ");

						if(counter+1!=noOfWords){
							i++;
							counter++;
							flagCounterIncreased = 1;
						}
						else {
							break;
						}
					}
					else{				//if not found
						flagStringWasNotFound = 1;
						break;
					}	
					counterForArray++;
					flagFinalString = 0;
				}
				if(flagStringWasNotFound){
					for(int m=0; m<=tempElement->staticArrayLength; m++){
						free(convertedStrings[m]);
						convertedStrings[m] = NULL;
					}
					free(convertedStrings);
					convertedStrings = NULL;
					break;
				}
				if(flagCounterIncreased){
					flagCounterIncreased = 0;
					i--;
					counter--;
				}
			}
			else {					// if tempElement is not a supernode
				
				char *word = getString(tempElement);
				if(strcmp(word,arrayOfWords[i])==0){
					if(counter!=0 && flagFinalString != 1 && !flagFound){
						strLength += strlen(arrayOfWords[i]) + 2;
						finalString = (char*)realloc(finalString,(strLength)*sizeof(char));

						finalString[strlen(finalString)] = '\0';
						strcat(finalString, convertedStrings[0]);	
					}
				
					if(tempElement->isFinal){
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
							else{		
								int itemPos = binarySearchTopK(topArray->array, finalString,0, topArray->positionInsertion-1);
								topArray->array[itemPos].occurences++;
							}
							
							returningStringLength += strlen(finalString)+2;
							returningString=realloc(returningString,returningStringLength *sizeof(char));
							strcat(returningString,finalString);

						}	
					}
					finalString = realloc(finalString,(strlen(finalString)+2)*sizeof(char));
					finalString[strlen(finalString)] = '\0';
					strcat(finalString, " ");
				}
				flagFinalString = 0;
			}
			
			
			
			
			for(int m=0; m<=tempElement->staticArrayLength; m++){
				free(convertedStrings[m]);
				convertedStrings[m] = NULL;
			}
			free(convertedStrings);
			convertedStrings = NULL;

			//get next tempElement
			tempArray = tempElement->nextWordArray;
			if(tempArray==NULL){
				break;
			}
			
			int position=i;
			if(counter+1!=noOfWords && flagCounterIncreased==0){
				 position = i+1;
			}
			else 
				subFlag=1;

			dataNode* madeUpElement = malloc(sizeof(dataNode));
			initializeDataNode(madeUpElement);
			insertString (madeUpElement, arrayOfWords[position]);
			checkItemExists* getPosition = binarySearch(tempArray, madeUpElement, 0 ,tempArray->position-1,NULL, false); 
			
			deleteDataNode(madeUpElement);
			free(madeUpElement);
			madeUpElement = NULL;
			if(getPosition->exists){
				tempElement = getPosition->insertedNode;
				free(getPosition);
				getPosition = NULL;
			}
			else{
				free(getPosition);
				getPosition = NULL;
				break;
			}
			
			flagFinalString = 0;
			counter++;
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
	filter = NULL;
	
	return returningString;	
				
}

//delete
void delete_ngram(HashTable* hashTable, char** arrayOfWords, int noOfWords, int deletionNum){
	arrayOfStructs* tempArray = NULL;
	stack* myStack = malloc(sizeof(stack));
	initializeStack(myStack);
	dataNode* lookupElement=NULL;
	bool doNotDelete = false;
	bool elementFinal = false;
	for(int i=0; i<noOfWords; i++){
		
		if(i==0){
			lookupElement = lookupTrieNode(arrayOfWords[i] ,hashTable);
			
			if(lookupElement==NULL){
				deleteStack(myStack);
				free(myStack);
				myStack = NULL;
				return;
			}

			if(lookupElement->nextWordArray!=NULL && i==noOfWords-1){			
				doNotDelete = true;
			}
			
			elementFinal = lookupElement->isFinal;
			
			if(i==noOfWords-1){
				lookupElement->notFinalInDeletionVersion = deletionNum;
				elementFinal = false;
			}
		}
		else{
			if(i==1){
				tempArray = lookupElement->nextWordArray;
				if(tempArray == NULL)
				{
					deleteStack(myStack);
					free(myStack);
					myStack = NULL;
					return;				
				}	
			}
			int position = tempArray->position;
		
			dataNode* tempElement = malloc(sizeof(dataNode));
			tempElement->staticArray = NULL;
			tempElement->staticArrayLength = 0;
			insertString (tempElement, arrayOfWords[i]);
			tempElement->nextWordArray=NULL;
			
			//find out if word exists in array and if it does return position
			checkItemExists* getPosition = binarySearch(tempArray, tempElement,0 ,tempArray->position-1,NULL, false);
			
			if(	getPosition->exists==true){
				push(myStack, getPosition->position);
				if(i==noOfWords-1){
					tempArray->array[getPosition->position].notFinalInDeletionVersion = deletionNum;
				}
			}
			else{										//element was not found inside array so it can not be deleted	
				doNotDelete = true;
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
	int flagNextArrayWasDeleted = 0;
	int counter = 0;
	while(!isEmpty(myStack)){
		//root of trie
		
		if(strcmp(arrayOfWords[0],"the")==0){
		}
		
		counter++;
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
			flagNextArrayWasDeleted = 1;
		}
		//if element not last && element.isFinal()
		
		if (flagLastElementDeleted ==1 && tempArray->array[myStack->positionsToDelete[myStack->top]].isFinal){
			doNotDelete = true;
			flagIfElementWasDeleted=0;
			break;
		}
		
		
		if(tempArray->array[myStack->positionsToDelete[myStack->top]].nextWordArray == NULL || hasChildren(lookupElement, deletionNum)==0){			//hasnt got children
			tempArray->array[myStack->positionsToDelete[myStack->top]].deletionVersion = deletionNum;
			
			flagIfElementWasDeleted=1;
			flagLastElementDeleted = 1;

		}else{
			flagIfElementWasDeleted=0;
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
	
	if(isEmpty(myStack)==1 && counter!=0){
		doNotDelete = false;
	}
	else{
		doNotDelete = true;
	}
	
	if(hasChildren(lookupElement, deletionNum) || elementFinal){
		doNotDelete = true;
	}
	else{
		doNotDelete = false;
	}

	if(!doNotDelete){
		char* lookUpWord = getString(lookupElement);
		int bucket = getBucketFromHash(hashTable->level, hashTable->initialLength, hashTable->bucketToBeSplit, lookUpWord, lookupElement->noOfChars);	
		int cell = getCell(lookUpWord , hashTable,bucket);
		hashTable->buckets[bucket].cells[cell].deletionVersion = deletionNum;

	}

	deleteStack(myStack);
	free(myStack);
	myStack = NULL;
	
}
