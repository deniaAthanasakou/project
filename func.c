#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "func.h"
#include "stack.h"
#include "bloomfilter.h"

void insert_ngram(HashTable* hashTable, char** arrayOfWords, int noOfWords){		//same layer
	arrayOfStructs* tempArray =NULL ;//= array_of_structs;
	int i=0;
	
	//printf("========================================================================\n");
	dataNode* insertedElement = NULL;
	
	for(i=0; i<noOfWords; i++){				//different layers
	//printf("INSERTING WORD '%s\n",arrayOfWords[i]);
		if(i==0){			//root word
			printf("INSERTING in trie WORD '%s\n",arrayOfWords[i]);
			dataNode* rootElement=NULL;
			rootElement = (dataNode*)malloc(1*sizeof(dataNode));		//creating dataNode for insert
			initializeDataNode(rootElement);
			if(i==noOfWords -1)		//final word
				rootElement->isFinal=true;
			else
				rootElement->isFinal=false;

			insertString (rootElement, arrayOfWords[i]);
			
			printf("before insert trie\n");
			insertedElement = insertTrieNode(rootElement, hashTable);					//inserting node into hashTable
			
			
			printf("word %s\n",insertedElement->word);
			
			deleteDataNode(rootElement);
			free(rootElement);
			rootElement=NULL;	
			//printf("DONE\n");
		}
		else{
			

			dataNode* tempElement=NULL;
			tempElement = (dataNode*)malloc(1*sizeof(dataNode));		//creating dataNode for insert
			
			//printf("@@@@@@@@ After malloc tempEl %p insertedElement %p word '%s'\n",tempElement,insertedElement,insertedElement->word);
			initializeDataNode(tempElement);
			if(i==noOfWords -1)		//final word
				tempElement->isFinal=true;
			else
				tempElement->isFinal=false;

			insertString (tempElement, arrayOfWords[i]);
			
			//printf("INSERTED ELEMENT AFTER INSERTSTRING %s\n", insertedElement->word);
			
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
				//printf("--------------INSERT '%s'\n",arrayOfWords[i]);
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
	//printf("element: '%s' printing next words\n",insertedElement->word);
	//if(insertedElement->nextWordArray!=NULL){
	//	printFullArray(insertedElement->nextWordArray, insertedElement->nextWordArray->position);
	//}
		
		
	
}

//search
char* search_ngram(HashTable *hashTable, char** arrayOfWordsOriginal, int noOfWordsOriginal, BloomFilter* topFilter, topKStruct* topKArray){		//is called for a single query

	BloomFilter* filter = initializeFilter(5);		//initialize bloomFilter here
	
	char** finalStringArray=malloc(0*sizeof(char*));
	int itemsOffinalStringArray=0;
	char* returningString=malloc(1*sizeof(char));
	strcpy(returningString,"");			//initialization
	int returningStringLength=0;
	int found = 0;
	int noOfWords=noOfWordsOriginal;
	for(int j=0; j < noOfWordsOriginal; j++){	//for each word of query starting as first Word
		
		//printf("word is '%s'\n",arrayOfWordsOriginal[j]);
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
				//printf("first word is '%s'\n",arrayOfWords[i]);
				firstElement = lookupTrieNode(arrayOfWords[i],hashTable);
				//printf("after lookup \n");
				if(firstElement==NULL){
				//	printf("NULL ELEMENT \n");
					break;
				}
				strLength += strlen(arrayOfWords[i]) + 2;
				finalString = (char*)realloc(finalString,(strLength)*sizeof(char));
				strcpy(finalString,arrayOfWords[i]);
				
				if(firstElement->isFinal){
					found=1;
					//printf("found\n");
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
					
					//break;
				}
				
				//printf("finalString is %s\n",finalString);
				finalString = realloc(finalString,(strlen(finalString)+2)*sizeof(char));
				finalString[strlen(finalString)] = '\0';
				strcat(finalString, " ");
			}
			else{
			//	printf("next word is '%s'\n",arrayOfWords[i]);
				//printf("finalString %s\n",finalString);
				if(i==1){
					tempArray = firstElement->nextWordArray;
					if(tempArray == NULL)
					{
						//deleteDataNode(firstElement);
						////printf("aa\n");
						//free(firstElement);
						//printf("bb\n");
						//firstElement=NULL;
						break;		
					}

				}
				dataNode* tempElement = malloc(sizeof(dataNode));
				insertString (tempElement, arrayOfWords[i]);
				//tempElement->word= (char*)malloc((strlen(arrayOfWords[i])+1) * sizeof(char));
				//strcpy(tempElement->word,arrayOfWords[i]);
				//printf("before binary\n");
				//if(tempArray==NULL) printf("aaaaaaaaaaaaaaaaaaaaaaaaaxxxxxxxxxxxxxxxxx\n");
				checkItemExists* getPosition = binarySearch(tempArray, tempElement, 0 ,tempArray->position,NULL); 
				//printf("after binary\n");
				if(getPosition->exists==true){		//if word was found
				//	printf("FOUND\n");
					//printf("finalString %s\n",finalString);
					strLength += strlen(arrayOfWords[i]) + 2;
					finalString = (char*)realloc(finalString,(strLength)*sizeof(char));

					
				
					finalString[strlen(finalString)] = '\0';
					char* word = getString(&(tempArray->array[getPosition->position]));
					strcat(finalString, word);	
					free(word);
					word = NULL;

					//printf("after strcat with %s\n",tempArray->array[getPosition->position].word);

					
					if(tempArray->array[getPosition->position].isFinal == true){		//if true check whether finalString should be printed
						found=1;
						//printf("found\n");
						//printf("FOUND INSIDE IF\n");
						//if(!checkIfStringExists(finalStringArray,itemsOffinalStringArray, finalString)){		//finalString does not exist in finalStringArray
						if(!bloomFilterSeach(filter,finalString)){			//if finalString should be printed (is not in filter)
						//if(!bloomFilterSearch(finalString)){			//if finalString should be printed (is not in filter)
							//printf("inside bloom\n");
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
						//printf("outside bloom\n");


					}
					//printf("realloc for final\n");
					finalString = realloc(finalString,(strlen(finalString)+2)*sizeof(char));
					finalString[strlen(finalString)] = '\0';
					strcat(finalString, " ");
					//printf("aaaaaaaaaa\n");
				}
				else{
					//printf("not found\n");
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
					//printf("aa\n");
					free(tempElement);
					//printf("bb\n");
					tempElement=NULL;
					free(getPosition);
					//printf("cc\n");
					getPosition = NULL;
					break;		
				}

				//printf("before free datanode\n");
				deleteDataNode(tempElement);
				//printf("a\n");
				free(tempElement);
				//printf("b\n");
				tempElement=NULL;
				free(getPosition);
				getPosition = NULL;
				//printf("end\n");
			
			}
		}
		//printf("benddddddddddd\n");
		free(finalString);
		finalString = NULL;
		noOfWords--;
		
		//printf("enddddddddddd\n");
	}
	
	//printf("outsideeeeeeeeee\n");
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
void delete_ngram(HashTable* hashTable, char** arrayOfWords, int noOfWords){
	//printf("Before\n");
	//printFullArray(array_of_structs,array_of_structs->position);
	arrayOfStructs* tempArray = NULL;
	stack* myStack = malloc(sizeof(stack));
	initializeStack(myStack);
	dataNode* lookupElement=NULL;
	bool doNotDelete = false;
	for(int i=0; i<noOfWords; i++){
	
	
		if(i==0){
			//lookup node
			lookupElement = lookupTrieNode(arrayOfWords[i] ,hashTable);
			if(lookupElement==NULL){
			//	printf("lookupElement==NULL\n");
				deleteStack(myStack);
				free(myStack);
				myStack = NULL;
				return;
			}
			
			if(lookupElement->nextWordArray!=NULL){
				doNotDelete = true;
			}
		//	printf("lookupElement->word = %s\n",lookupElement->word);
			
		//	printf("\n\n\n");
			//printBuckets(hashTable);
			//printf("\n\n\n");
			
			if(i==noOfWords-1){
				lookupElement->isFinal=false;
			}
			//if not found return
			//else
			//keepit
		}
		else{
			if(i==1){
				tempArray = lookupElement->nextWordArray;
			}

			int position = tempArray->position;
		
			dataNode* tempElement = malloc(sizeof(dataNode));
			insertString (tempElement, arrayOfWords[i]);
			//tempElement->word= (char*)malloc((strlen(arrayOfWords[i])+1) * sizeof(char));
			tempElement->nextWordArray=NULL;
			//strcpy(tempElement->word,arrayOfWords[i]);	
		
			//printf("tempElement->word = %s\n",tempElement->word);
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
	
	//displayStack(myStack);
	
	//must delete from stack and must delete from bucket
//	printf("before\n");
//	printFullArray(lookupElement->nextWordArray,lookupElement->nextWordArray->position);
	
	int flagIfElementWasDeleted=0;
	int flagLastElementDeleted = 0;
	//printf("before stack\n");
	while(!isEmpty(myStack)){
		//root of trie
		//tempArray = array_of_structs;
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
		//printf("hasnt got children\n");
		//if(tempArray->array[myStack->positionsToDelete[myStack->top]].nextWordArray == NULL){			//hasnt got children
			//printf("worddd: %s\n",tempArray->array[myStack->positionsToDelete[myStack->top]].word);
			deletionSort(tempArray,myStack->positionsToDelete[myStack->top], tempArray->position);
			flagIfElementWasDeleted=1;
			flagLastElementDeleted = 1;
		}else{	
			//printf("has children\n");
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
	//printf("After\n");
	//printFullArray(lookupElement->nextWordArray,lookupElement->nextWordArray->position);
	
	//must call deletionSort for bucket
	//lookUpBucket->bucket
	//binarySearch->position
	//deletionSortBucket(Bucket* bucket, int position);
	
	if(!doNotDelete){
		char* lookUpWord = getString(lookupElement);
		int bucket = getBucketFromHash(hashTable->level, hashTable->initialLength, hashTable->bucketToBeSplit, lookUpWord);
		//printf("bucket is %d\n", bucket);
	
		int cell = getCell(lookUpWord , hashTable,bucket);
	
	//	printf("cell is %d\n", cell);
	
		deletionSortBucket(&hashTable->buckets[bucket], cell);
		
		free(lookUpWord);
		lookUpWord = NULL;
	}
	
	
	deleteStack(myStack);
	free(myStack);
	myStack = NULL;
	
	
	//printf("\n\n\n AFTER DELETE\n");
	//printBuckets(hashTable);
	//printf("\n\n\n");
	
	
	
}


































