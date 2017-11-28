#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "func.h"
#include "stack.h"
#include "bloomfilter.h"

void insert_ngram(HashTable* hashTable, char** arrayOfWords, int noOfWords){		//same layer
	arrayOfStructs* tempArray =NULL ;//= array_of_structs;
	int i=0;
	dataNode* rootElement=NULL;
	dataNode* tempElement=NULL;
	dataNode* insertedElement = NULL;
	
	for(i=0; i<noOfWords; i++){				//different layers
		//printf("INSERTING WORD '%s\n",arrayOfWords[i]);
		if(i==0){			//root word
		
		
			rootElement = malloc(sizeof(dataNode));		//creating dataNode for insert
			if(i==noOfWords -1)		//final word
				rootElement->isFinal=true;
			else
				rootElement->isFinal=false;
		
			insertString (rootElement, arrayOfWords[i]);
			rootElement->nextWordArray=NULL;
		
			insertedElement = insertTrieNode(rootElement, hashTable);					//inserting node into hashTable
			
			deleteDataNode(rootElement);
			free(rootElement);
			rootElement=NULL;	
			
		}
		else{
		
			//if(insertedElement->nextWordArray!=NULL){
			//	printf("printing array of word '%s'\n",insertedElement->word);
				//printFullArray(insertedElement->nextWordArray, insertedElement->nextWordArray->position);
			//	printf("AFTER printing array of word '%s'\n",insertedElement->word);
			//}
		
			tempElement = malloc(sizeof(dataNode));		//creating dataNode for insert
			if(i==noOfWords -1)		//final word
				tempElement->isFinal=true;
			else
				tempElement->isFinal=false;
		
			insertString (tempElement, arrayOfWords[i]);
			tempElement->nextWordArray=NULL;
			
			//printf("this\n");
			
			if(i==1){
				//printf("i==1\n");
				if(insertedElement->nextWordArray==NULL){
					//printf("nextWordArray==NULL\n");
					insertedElement->nextWordArray =  malloc(1 * sizeof(arrayOfStructs));
					initializeArray(insertedElement->nextWordArray);
				}
				tempArray = insertedElement->nextWordArray;	
				//tempArray = malloc(1 * sizeof(arrayOfStructs));
					
			}
			checkItemExists* getPosition = insertionSort(tempArray, tempElement, tempArray->position); 
	
			//printf("position is %d\n", getPosition->position);
		
			if(getPosition->exists==true){
				//printf("already exists\n");
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
				//printf("WORD IS '%s' tempArray->position is %d\n",tempArray->array[1].word, tempArray->position);
			}
			if(i!=noOfWords -1 && tempArray->array[getPosition->position].nextWordArray==NULL){
				//printf("inside if\n");
				tempArray->array[getPosition->position].nextWordArray = malloc(1 * sizeof(arrayOfStructs));
				initializeArray(tempArray->array[getPosition->position].nextWordArray);

			}
			
			//printFullArray(tempArray, tempArray->position);
			free(tempElement);
			tempElement=NULL;
			tempArray = tempArray->array[getPosition->position].nextWordArray;
			//printf("next tempARray\n");
			free(getPosition);
			getPosition = NULL;
		
		}
		
	}
	/*printf("element: '%s' printing next words\n",insertedElement->word);
	if(insertedElement->nextWordArray!=NULL){
		printFullArray(insertedElement->nextWordArray, insertedElement->nextWordArray->position);
	}*/
		
		
		
		
	
}

//search
char* search_ngram(HashTable *hashTable, char** arrayOfWordsOriginal, int noOfWordsOriginal){		//is called for a single query

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
				if(firstElement==NULL)
					break;
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
					
					break;
				}
				
				//printf("finalString is %s\n",finalString);
				finalString = realloc(finalString,(strlen(finalString)+2)*sizeof(char));
				finalString[strlen(finalString)] = '\0';
				strcat(finalString, " ");
			}
			else{
				//printf(" word is '%s'\n",arrayOfWords[i]);
				//printf("finalString %s\n",finalString);
				if(i==1)
					tempArray = firstElement->nextWordArray;
				dataNode* tempElement = malloc(sizeof(dataNode));
				insertString (tempElement, arrayOfWords[i]);
				//tempElement->word= (char*)malloc((strlen(arrayOfWords[i])+1) * sizeof(char));
				//strcpy(tempElement->word,arrayOfWords[i]);
				//printf("before binary\n");
				//if(tempArray==NULL) printf("aaaaaaaaaaaaaaaaaaaaaaaaaxxxxxxxxxxxxxxxxx\n");
				checkItemExists* getPosition = binarySearch(tempArray, tempElement, 0 ,tempArray->position,NULL); 
				//printf("after binary\n");
				if(getPosition->exists==true){		//if word was found
					//printf("FOUND\n");
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
































