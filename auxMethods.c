#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "auxMethods.h"
#include "struct.h"


int initialize(FILE* file, HashTable* hashTable){
	char *line = NULL;
	size_t len = 0;
	int read;
	int counter = 0;
	int returnValue = 1;		//default
	if (file == NULL)
		return returnValue;
	while ((read = getline(&line, &len, file)) != -1) {
		char* ngram = strtok(line, "\n");
		if(strcmp(ngram,"\n")==0 || strcmp(ngram,"\t")==0 || strcmp(ngram," ")==0){
			printf("WWWWWP\n");
			exit(1);
		}
		if (counter == 0){
			if(strcmp(ngram,"STATIC")==0)
				returnValue = 0;
			else if (strcmp(ngram,"DYNAMIC")==0)
				returnValue = 1;
			else{
				printf("Error with init file!\n");
				exit(1);
			}
				
		}
		else{
			callBasicFuncs(ngram,'A',hashTable,NULL,NULL,returnValue);
		}
		counter++;
	}
	//printf("EOF\n");

	if (line){
		free(line);
		line=NULL;
	}
	return returnValue;
}


void callBasicFuncs(char* ngram, char query , HashTable* hashTable, BloomFilter* topFilter, topKArray *topArray, int isDynamic){

	arrayWords* arrayW = stringToArray(ngram);
	int noOfWords = arrayW->length;
	char** arrayOfWords = arrayW->words;
	if(query == 'A'){
		insert_ngram(hashTable, arrayOfWords,noOfWords);
	}
	else if(query == 'Q'){
		char* searchString = NULL;
		if(isDynamic){
		 	searchString= search_ngram(hashTable, arrayOfWords,noOfWords, topFilter, topArray);
		 }
		else{
			searchString = search_ngram_StaticVersion(hashTable, arrayOfWords,noOfWords, topFilter, topArray);
		}
		if(searchString!=NULL){
			free(searchString);
			searchString=NULL;
		}
	}
	else if(query == 'D'){
		delete_ngram(hashTable, arrayOfWords,noOfWords);
	}
	
	deleteArrayOfWords(arrayOfWords,noOfWords);
	if(arrayOfWords!=NULL){
		free(arrayOfWords);
		arrayOfWords=NULL;
		free(arrayW);
		arrayW = NULL;
	}
	
}

int checkIfStringExists(char** array, int noOfWords, char* str){
	
	for(int i=0; i<noOfWords; i++){
		if(strcmp(str,array[i])==0)
			return 1;
	}
	return 0;
}



arrayWords* stringToArray(char* ngram){
	char* pch;
	char** arrayOfWords; 
	pch = strtok (ngram," \t");
	int noOfWords=0;
	int length = 10;
	arrayOfWords = malloc(length * sizeof(char*));
	while (pch != NULL)
	{	
		if(noOfWords == length){
			length += 10;
			arrayOfWords = (char**)realloc(arrayOfWords, length * sizeof(char*));	
		}
		arrayOfWords[noOfWords]=pch;
		pch = strtok (NULL, " \t");
		noOfWords++;
	}
	
	arrayWords* arrayW = malloc(sizeof(arrayWords));
	arrayW->length = noOfWords;
	arrayW->words = arrayOfWords;
	
	return arrayW;

}

void copyDataNode(dataNode* node, dataNode* tempNode){

	node->nextWordArray = tempNode->nextWordArray;
	node->isFinal = tempNode->isFinal;
	node->isDynamic = tempNode->isDynamic;
	if(tempNode->dynamicWord!=NULL)
		strcpy(node->dynamicWord , tempNode->dynamicWord);
	else
		node->dynamicWord=NULL;	
	node->noOfChars = tempNode->noOfChars;
	for(int i=0; i<node->noOfChars; i++){
		node->word[i] = tempNode->word[i];
	}

	node->staticArray = tempNode->staticArray;
	node->staticArrayLength = tempNode->staticArrayLength;
	
}


checkItemExists* binarySearch(arrayOfStructs* array_of_str, dataNode* item, int first, int last, checkItemExists* check)	
{
	if(check==NULL)
		check = malloc(sizeof(checkItemExists));
	check->exists=false;
	dataNode* array = array_of_str->array;
    if (last < first){
    	check->exists=false;
    	char* wordFirstMalloc = NULL;
    	if(array[first].noOfChars!=-1){
			char* wordFirst = getString(&(array[first]));
			
			
		 	if(array[first].staticArray!=NULL){
				int sizeMalloc = abs(array[first].staticArray[0]);
				wordFirstMalloc = malloc((sizeMalloc+1)*sizeof(char));
				int j = 0;
				int k=0;
				for(j=0; j< sizeMalloc; j++){
					wordFirstMalloc[j] = array[first].dynamicWord[k];
					k++;
				}
				wordFirstMalloc[j] = '\0';
			}
			else{
				wordFirstMalloc = malloc((array[first].noOfChars+1)*sizeof(char));
				strcpy(wordFirstMalloc,wordFirst);
			}

			char* wordItem = getString(item);
			
			if(wordFirstMalloc!=NULL){
				if(strcmp(wordItem,wordFirstMalloc)>0){
					check->position=first + 1;
				}
				else{
					check->position=first;
				}
		
			}else{
				check->position = first+1;
			}
			
			free(wordFirstMalloc);
			wordFirstMalloc = NULL;
			
    	}
    	else{
			check->position = first+1;
		}
    	return 	check;
    	wordFirstMalloc = NULL;
     }

 	int mid = (first+last)/2;

	//char* string = array[mid].dynamicWord;
	//int numm = array[mid].noOfChars;
	if(array[mid].isEmpty)
	{
		check->exists=false;
 		check->position=-1;
		return check;
	}
		
 	/*if(array[mid].noOfChars==-1){			
 		check->exists=false;
 		check->position=-1;
		return check;
 	}*/
 	
 	char* wordMid = getString(&(array[mid]));
 	char* wordMidMalloc = NULL;
 	if(array[mid].staticArray!=NULL){
		int sizeMalloc = abs(array[mid].staticArray[0]);
		wordMidMalloc = malloc((sizeMalloc+1)*sizeof(char));
		int j = 0;
		for(j=0; j< sizeMalloc; j++){
			wordMidMalloc[j] = array[mid].dynamicWord[j];
		}
		wordMidMalloc[j] = '\0';
	}
	else{
		wordMidMalloc = malloc((array[mid].noOfChars+1)*sizeof(char));
		strcpy(wordMidMalloc,wordMid);
	}
	

	char* wordItem = getString(item);
 	
	if(strcmp(wordItem ,wordMidMalloc)==0){
		check->position=mid;
		check->exists=true;
		check->insertedNode = &array[mid];
		
		if(wordMidMalloc!=NULL){
			free(wordMidMalloc);
			wordMidMalloc = NULL;
		}
	    return check;
	 }
	 
 
	if(strcmp(wordItem,wordMidMalloc)>0){
		if(wordMidMalloc!=NULL){
			free(wordMidMalloc);
			wordMidMalloc = NULL;
		}
	    return binarySearch(array_of_str, item, mid+1, last, check);
	}
	
	if(wordMidMalloc!=NULL){
		free(wordMidMalloc);
		wordMidMalloc = NULL;
	}
	return binarySearch(array_of_str, item, first, mid-1, check);


}
 
 
// Function to sort an array_of_str->array  of size 'n'
checkItemExists* insertionSort(arrayOfStructs* array_of_str, dataNode* itemForInsert, int lastElement)
{
    int i, loc, j;
    
    checkItemExists* retPosition=malloc(sizeof(checkItemExists));
    retPosition->position=0;	//insert in first elements
    retPosition->exists=false;
    
	if(lastElement==0){
		memmove(&(array_of_str->array[0]), itemForInsert, sizeof(*itemForInsert));
		return retPosition;
	}
	free(retPosition);
	retPosition=NULL;
    j = lastElement - 1;

    // find location where selected sould be inseretd
    checkItemExists* getPosition = binarySearch(array_of_str, itemForInsert, 0, j,NULL);
    if(getPosition->exists==true){
    	return getPosition;
    }

	//double array
	if(array_of_str->position == array_of_str->length-1){
		doubleLength(array_of_str);
	}

    loc=getPosition->position;
    
    // Move all elements after location to create space
    int fullMoveSize=0;
	int startingPoint = getPosition->position;	
	if( j>=loc){
	
		fullMoveSize = (lastElement -loc)*sizeof(array_of_str->array[lastElement - 1]);
		memmove(&(array_of_str->array[startingPoint+1]), &(array_of_str->array[startingPoint]), fullMoveSize);
		memmove(&(array_of_str->array[loc]), itemForInsert, sizeof(*itemForInsert));
		getPosition->position=loc;
	}
	else{
		memmove(&(array_of_str->array[lastElement]), itemForInsert, sizeof(*itemForInsert));
   		getPosition->position=lastElement;
	}	
    
    return getPosition;  
}



 

checkItemExists* binarySearchBucket(dataNode* array, dataNode* item, int first, int last, int trueLastElement, checkItemExists* check)	
{
	if(check==NULL)
		check = malloc(sizeof(checkItemExists));

	check->exists=false;
	check->insertedNode = NULL;
	
    if (last < first){
    	if( first >= trueLastElement){
    		check->position = first+1;
    		return 	check;
    	}
    		
    	check->exists=false;
    	if(array[first].noOfChars!=-1){
			char* wordFirst = getString(&(array[first]));
			char* wordItem = getString(item);
			if(wordFirst!=NULL){
				
				if(strcmp(wordItem,wordFirst)>0){
					check->position=first + 1;
				}
				else{
					check->position=first;
				}
		
			}else{
				check->position = first+1;
			}	
    	}
    	else{
			check->position = first+1;
		}
	
    	return 	check;
     }

 	int mid = (first+last)/2;
		
 	if(array[mid].noOfChars==-1){			
 		check->exists=false;
 		check->position=-1;
		return check;
 	}
 	
 	
 	char* wordMid = getString(&(array[mid]));
	char* wordItem = getString(item);
 	
	if(strcmp(wordItem ,wordMid)==0){
		check->position=mid;
		check->exists=true;
	    return check;
	 }
 
	if(strcmp(wordItem,wordMid)>0){
	    return binarySearchBucket(array, item, mid+1, last,trueLastElement, check);
	}
	return binarySearchBucket(array, item, first, mid-1,trueLastElement, check);
}
 


checkItemExists* insertionSortBucket(HashTable* hashTable,Bucket* bucket, dataNode* itemForInsert, int lastElement)
{	
	int  loc, j; 
    checkItemExists* retPosition=malloc(sizeof(checkItemExists));
    retPosition->position=0;	//insert in first elements
    retPosition->insertedNode = NULL;
    retPosition->exists=false;
	
	if(lastElement==0){
		memmove(&(bucket->cells[0]), itemForInsert, sizeof(*itemForInsert));
		bucket->position++;
		retPosition->insertedNode = &(bucket->cells[0]);
		return retPosition;
	}
	free(retPosition);
	retPosition=NULL;
    j = lastElement - 1;

    // find location where selected should be inserted
    checkItemExists* getPosition = binarySearchBucket(bucket->cells, itemForInsert, 0, j,bucket->position, NULL);
    if(getPosition->exists==true){
    	if(itemForInsert->isFinal)
    		bucket->cells[getPosition->position].isFinal= true;
    	
    	getPosition->insertedNode = &(bucket->cells[getPosition->position]);
    	return getPosition;
    }
	//increase cells
	if(bucket->position == bucket->length){
		createOverflowCells(bucket);
	}

    loc=getPosition->position;

    
    // Move all elements after location to create space
    int fullMoveSize=0;
	int startingPoint = getPosition->position;		
	if( j>=loc){
		fullMoveSize = (lastElement -loc)*sizeof(bucket->cells[lastElement - 1]);
		memmove(&(bucket->cells[startingPoint+1]), &(bucket->cells[startingPoint]), fullMoveSize);
		memmove(&(bucket->cells[loc]), itemForInsert, sizeof(*itemForInsert));
		getPosition->insertedNode = &(bucket->cells[loc]);
    	getPosition->position=loc;
	}
	else{
		memmove(&(bucket->cells[lastElement]), itemForInsert, sizeof(*itemForInsert));
		getPosition->insertedNode = &(bucket->cells[lastElement]);
   		getPosition->position=lastElement;
	}
	bucket->position++;
    return getPosition;
}


void deletionSort(arrayOfStructs* array_of_str,	int position, int lastElement){
	
	if(array_of_str->array[position].dynamicWord!=NULL){		
		free(array_of_str->array[position].dynamicWord);
		array_of_str->array[position].dynamicWord=NULL;
	}
	else{
		array_of_str->array[position].word[0] = '\0';
	}
	if(array_of_str->array[position].nextWordArray!=NULL){
		deleteArray(array_of_str->array[position].nextWordArray);
		array_of_str->array[position].nextWordArray=NULL;
	}
			
	int fullMoveSize = 0;
	if(position < array_of_str->position -1){
		fullMoveSize = (array_of_str->position -1 -position)*sizeof(array_of_str->array[position+1]);
		memmove(&(array_of_str->array[position]),&(array_of_str->array[position+1]), fullMoveSize);
	}
	array_of_str->position--;
}


 

void printFullArray(arrayOfStructs* array_of_str, int position){	//prints all layers
	arrayOfStructs* tempArray = array_of_str;
	printf("PRINTING FULL ARRAY\n");
	if(tempArray != NULL){
		int lastElement = position;	
		for(int i=0; i < lastElement; i++){
			char* word = getString(&( tempArray->array[i]));
			printf("'%s' is in position %d, ", word, i);
			if(i==lastElement-1){				//print only once
				printf("\b\b: ");
				printArray(tempArray,(position-1));
			}
			if( tempArray->array[i].nextWordArray!=NULL){
				printFullArray( tempArray->array[i].nextWordArray,  tempArray->array[i].nextWordArray->position);
				
			}
		}
	}	
}

void printArray(arrayOfStructs* array_of_str, int position){		//prints layer

	printf("ELEMENTS ARE: [");
	for(int k=0; k< array_of_str->position;k++){
		char* word = getString(& (array_of_str->array[k]));
		printf("%s ",  word);
		if(k!=array_of_str->position-1)
			printf(", ");
	}
	printf("]\n");
}

void printArrayFinalWords(arrayOfStructs* array_of_str, int position){

	printf("FINAL ELEMENTS ARE: [");
	for(int k=0; k< array_of_str->position;k++){
		if(array_of_str->array[k].isFinal){
			char* word = getString(& (array_of_str->array[k]));
			printf("%s ",  word);
		
			if(k!=array_of_str->position)
				printf(", ");
		}
	}
	printf("]\n");
}



//insert from query file
int executeQueryFile(FILE* file, HashTable* hashTable, int staticDynamic){
	
	char *line = NULL;
	size_t len = 0;
	long long read;

	if (file == NULL)
		return 1;
		
	char endingLetter = 'F';			//initialization	
	int counter=-1;
	BloomFilter* topFilter = NULL;
	topKArray* topArray = NULL;
	while ((read = getline(&line, &len, file)) != -1) {
		char* ngram = strtok(line, "\n");
		if(ngram==NULL){
			continue;
		}
		counter++;
		if(counter == 0){		 
			topFilter = initializeFilter(5);		//initialize bloomFilter here
			topArray = initializeTopKArray();		//initialize topKArray
		}
		
		
		//find first letter for each case
		char* wordCase = strtok(ngram," \t");
		char* remainingLine = strtok(NULL,"");

		if(strcmp(wordCase,"A")==0){	
			endingLetter = 'A';
			if(staticDynamic==0){	//STATIC
				printf("Error with init file! Add is not supported in Static version.\n");
				if (line){
					free(line);
					line=NULL;
				}
				destroyLinearHash(hashTable);
				//free bloomFilter
				freeFilter(topFilter);
				//free array
				destroyTopArray(topArray);
				exit(1);
			}
			callBasicFuncs(remainingLine,'A',hashTable, NULL, NULL, 1);
		}
		else if(strcmp(wordCase,"Q")==0){
			
			endingLetter = 'Q';
			callBasicFuncs(remainingLine,'Q',hashTable,topFilter,topArray,staticDynamic);	
		}
		else if(strcmp(wordCase,"D")==0){
			endingLetter = 'D';
			if(staticDynamic==0){	//STATIC
				printf("Error with init file! Delete is not supported in Static version.\n");
				if (line){
					free(line);
					line=NULL;
				}
				destroyLinearHash(hashTable);
				
				//free bloomFilter
				freeFilter(topFilter);
				//free array
				destroyTopArray(topArray);
				
				exit(1);
			}
			callBasicFuncs(remainingLine,'D',hashTable, NULL, NULL, 1);
		}
		else if(strcmp(wordCase,"F")==0){
			endingLetter = 'F';
			counter = -1;
			
			
			
			//get top-k
			if(remainingLine!=NULL){
				int topK = atoi(remainingLine);		
				if(topK <= topArray->positionInsertion){
					HeapSort(topArray->array, topArray->positionInsertion, topK);	//sort based on integers	
					//bubbleSort(topArray->array, topArray->positionInsertion, topK);		//faster
					//print topK
				
					printTopK(topArray,topK);
				}
				
			}
			
			//free bloomFilter
			freeFilter(topFilter);
			//free array
			destroyTopArray(topArray);
			
		}
		else{			//different letter
			if (line){
				free(line);
				line=NULL;
			}
			
			destroyLinearHash(hashTable);
			//free bloomFilter
			freeFilter(topFilter);
			//free array
			destroyTopArray(topArray);
			
			return 0;
		}
		
	}//found eof
	if (line){
		free(line);
		line=NULL;
	}
	
	destroyLinearHash(hashTable);
	if(endingLetter !='F'){
		//free bloomFilter
		freeFilter(topFilter);
		//free array
		destroyTopArray(topArray);
	
		return 0;
	}

	return 1;

}

void printQuery(char** items, int iterNum){
	for(int i=0;i<iterNum;i++){
		if(i==iterNum-1){
			printf("%s\n",items[i]);
		}
		else{
			printf("%s|",items[i]);
		}		
	}

}


void insertString (dataNode* node, char* word){
	
	node->noOfChars = strlen(word) +1 ;
	
	if(strlen(word)<20){
		int i;
		for(i=0; i<strlen(word); i++){
			node->word[i] = word[i];
		}
		node->word[i]='\0';
		node->isDynamic = false;	
		node->dynamicWord = NULL;	
	}
	else{
		node->isDynamic = true;
		node->dynamicWord = malloc(sizeof(char) * node->noOfChars);
		strcpy(node->dynamicWord,word);
		node->word[0]='\0';
	}
	
	node->isEmpty = false;
	
}

char* getString(dataNode* node){
	char* returnWord = NULL;
	if(node->noOfChars==0)
		return returnWord;
	if(node->isDynamic){
		return node->dynamicWord;
	}
	
	return node->word;	
}

void deleteArrayOfWords(char** array,int length){		//is not used
}








