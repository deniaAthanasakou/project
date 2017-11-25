#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "auxMethods.h"
#include "struct.h"


void initialize(FILE* file, arrayOfStructs* structureTree, HashTable* hashTable){

	char *line = NULL;
	size_t len = 0;
	int read;

	if (file == NULL)
		return;
	while ((read = getline(&line, &len, file)) != -1) {	
		char* ngram = strtok(line, "\n");
		callBasicFuncs(ngram,structureTree,'A',hashTable);
	}
	//found eof

	if (line){
		free(line);
		line=NULL;
	}
}


void callBasicFuncs(char* ngram, arrayOfStructs* array, char query , HashTable* hashTable){

	arrayWords* arrayW = stringToArray(ngram);
	int noOfWords = arrayW->length;
	char** arrayOfWords = arrayW->words;
	if(query == 'A'){
		//addFilter(filter,ngram,strlen(ngram));
		insert_ngram(array, hashTable, arrayOfWords,noOfWords);
	}
	else if(query == 'Q'){
		char* searchString = search_ngram(array, arrayOfWords,noOfWords);
		free(searchString);
		searchString=NULL;
	}
	else if(query == 'D'){
		delete_ngram(array, arrayOfWords,noOfWords);
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
	arrayOfWords = malloc(noOfWords * sizeof(char*));
	while (pch != NULL)
	{	
		noOfWords++;
		arrayOfWords = (char**)realloc(arrayOfWords, noOfWords * sizeof(char*));		
		arrayOfWords[noOfWords-1]=malloc((strlen(pch)+1)* sizeof(char));
		strcpy(arrayOfWords[noOfWords-1],pch);    //add pch into arrayOfWords
		pch = strtok (NULL, " \t");
	}
	
	arrayWords* arrayW = malloc(sizeof(arrayWords));
	arrayW->length = noOfWords;
	arrayW->words = arrayOfWords;
	
	return arrayW;

}

void deleteArrayOfWords(char** array,int length){
	if(array!=NULL){
		for(int i=0;i<length;i++){
			if( array[i]!=NULL){
				free(array[i]);
				array[i] = NULL;
			}
		}
		
	}

}

checkItemExists* binarySearch(arrayOfStructs* array_of_str, dataNode* item, int first, int last, checkItemExists* check)	
{
	if(check==NULL)
		check = malloc(sizeof(checkItemExists));

	check->exists=false;
	
	dataNode* array = array_of_str->array;
    if (last < first){
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
		
			free(wordFirst);
			wordFirst = NULL;
			free (wordItem);
			wordItem=NULL;
			
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
	
		free(wordMid);
		wordMid = NULL;
		free (wordItem);
		wordItem=NULL;
	
	    return check;
	 }
 
	if(strcmp(wordItem,wordMid)>0){
	
		free(wordMid);
		wordMid = NULL;
		free (wordItem);
		wordItem=NULL;
	
	    return binarySearch(array_of_str, item, mid+1, last, check);
	}
	
	free(wordMid);
	wordMid = NULL;
	free (wordItem);
	wordItem=NULL;
	
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
		array_of_str->array[0] = *itemForInsert;
		return retPosition;
	}
	free(retPosition);
	retPosition=NULL;
	i=lastElement;
    j = i - 1;

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
    int moveSize=0;
	int startingPoint = j;
	if( j>=loc){
	while (j >= loc)
	{
	    array_of_str->array[j+1] = array_of_str->array[j];
		moveSize+=sizeof(array_of_str->array[j]);
	    j--;
	    
	}
		//printf("BEFOOOOOOOOOORE word %s with loc=%d and j=%d movesize %ld\n",itemForInsert->word,loc,startingPoint, moveSize);
		//printFullArray(array_of_str,array_of_str->position);
		//memmove(&(array_of_str->array[startingPoint+1]), &(array_of_str->array[startingPoint]), moveSize);
		//printf("AFTEEEEEEEEEEER\n");

	}
	array_of_str->array[j+1] = *itemForInsert;
    getPosition->position=j+1;
   /* if(startingPoint >=loc){
    	printFullArray(array_of_str,(array_of_str->position)+1);
    	printf("\n");
    }*/
    
    return getPosition;
}


checkItemExists* binarySearch2(dataNode* array, dataNode* item, int first, int last, checkItemExists* check)	
{
	if(check==NULL)
		check = malloc(sizeof(checkItemExists));

	check->exists=false;
	
	
    if (last < first){
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
		
			free(wordFirst);
			wordFirst = NULL;
			free (wordItem);
			wordItem=NULL;
			
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
	
		free(wordMid);
		wordMid = NULL;
		free (wordItem);
		wordItem=NULL;
	
	    return check;
	 }
 
	if(strcmp(wordItem,wordMid)>0){
	
		free(wordMid);
		wordMid = NULL;
		free (wordItem);
		wordItem=NULL;
	
	    return binarySearch2(array, item, mid+1, last, check);
	}
	
	free(wordMid);
	wordMid = NULL;
	free (wordItem);
	wordItem=NULL;
	
	return binarySearch2(array, item, first, mid-1, check);

}
 


checkItemExists* insertionSort2(Bucket* bucket, dataNode* itemForInsert, int lastElement)
{
	
	//printf("bucket->noOfElements %d\n",bucket->noOfElements+1);
	dataNode* array = malloc((bucket->noOfElements)*sizeof(dataNode));
	Bucket* tempBucket = bucket;
	int arrayCounter=0;
	int cellCounter=0;
	while(tempBucket!=NULL){		
		if(tempBucket->position == tempBucket->length){
			tempBucket=tempBucket->nextBucket;
			cellCounter = 0;
		}
		if(tempBucket->position == 0) {
			break;
		}	
			
		//printf("noOfElements %d\n",bucket->noOfElements+1);
		//printf("arrayCounter %d\n", arrayCounter);
		//printf("inner cell %")
		array[arrayCounter] = tempBucket->cells[cellCounter];
		arrayCounter++;
		cellCounter++;
	}
	
	//printf("after array\n");


    int i, loc, j;
    
    checkItemExists* retPosition=malloc(sizeof(checkItemExists));
    retPosition->position=0;	//insert in first elements
    retPosition->exists=false;
    
	if(lastElement==0){
		//bucket->cells[0] = *itemForInsert;
		array = realloc(array,(bucket->noOfElements +1 )*sizeof(dataNode));
		array[0] = *itemForInsert;
		printf("printing\n");
		for(int k=0; k<bucket->noOfElements +1; k++){
			printf("word '%s'\n", getString(&(array[k])));
		}
		return retPosition;
	}
	free(retPosition);
	retPosition=NULL;
	//i=lastElement;
    j = lastElement - 1;

    // find location where selected sould be inseretd
    checkItemExists* getPosition = binarySearch2(array, itemForInsert, 0, j,NULL);
    if(getPosition->exists==true){
    	return getPosition;
    }
    
    
    
    loc=getPosition->position;
    array = realloc(array,(bucket->noOfElements +1 )*sizeof(dataNode));
	int moveSize=0;
	int startingPoint = j;
	while (j >= loc)
	{
	   // array[j+1] = array[j];
	   moveSize+=sizeof(array[j]);
	    j--;
	    
	    
	}
	memmove(&array[startingPoint+1],&array[startingPoint],moveSize);
	array[j+1] = *itemForInsert;
	printf("printing\n");
	for(int k=0; k<bucket->noOfElements +1; k++){
		printf("word '%s'\n", getString(&(array[k])));
	}
	
	//create chunks
	//possibly free
	
	memmove((bucket->cells),&array[0],moveSize);
	
    getPosition->position=j+1;
    
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
	while (position < lastElement -1)
	{
	    array_of_str->array[position]=array_of_str->array[position+1];
	    position++;
	}

	array_of_str->position--;
     
}

void printFullArray(arrayOfStructs* array_of_str, int position){	//prints all layers
	arrayOfStructs* tempArray = array_of_str;
	
	if(tempArray != NULL){
		int lastElement = tempArray->position;
	
		for(int i=0; i < lastElement; i++){
			char* word = getString(&( tempArray->array[i]));
			printf("'%s' is in position %d, ", word, i);
			free(word);
			word=NULL;
			if(i==lastElement-1){				//print only once
				printf("\b\b: ");
				printArray(tempArray,(tempArray->position-1));
			}
			if( tempArray->array[i].nextWordArray!=NULL){
				printFullArray( tempArray->array[i].nextWordArray, tempArray->array[i].nextWordArray->position);
				
			}
		}
	}
		
}




void printArray(arrayOfStructs* array_of_str, int position){		//prints layer

	printf("ELEMENTS ARE: [");
	for(int k=0; k<= position;k++){
		char* word = getString(& (array_of_str->array[k]));
		printf("%s ",  word);
		
		free(word);
		word=NULL;
		
		if(k!=position)
			printf(", ");
	}
	printf("]\n");
	
	
}

void printArrayFinalWords(arrayOfStructs* array_of_str, int position){

	printf("FINAL ELEMENTS ARE: [");
	for(int k=0; k<= position;k++){
		if(array_of_str->array[k].isFinal){
			char* word = getString(& (array_of_str->array[k]));
			printf("%s ",  word);
		
			free(word);
			word=NULL;
		
			if(k!=position)
				printf(", ");
		}
	}
	printf("]\n");
	
	
}



//insert from query file
int executeQueryFile(FILE* file ,arrayOfStructs* structureTree , HashTable* hashTable){

	char *line = NULL;
	size_t len = 0;
	int read;

	if (file == NULL)
		return 1;
		
	char startingLetter = 'F';			//initialization			

	while ((read = getline(&line, &len, file)) != -1) {
	
		char* ngram = strtok(line, "\n");
		
		//find first letter for each case
		char* wordCase = strtok(ngram," \t");
		char* remainingLine = strtok(NULL,"");

		if(strcmp(wordCase,"A")==0){	
			startingLetter = 'A';
			callBasicFuncs(remainingLine,structureTree,'A',hashTable);
		}
		else if(strcmp(wordCase,"Q")==0){
			startingLetter = 'Q';
			callBasicFuncs(remainingLine,structureTree,'Q',hashTable);		
		}
		else if(strcmp(wordCase,"D")==0){
			startingLetter = 'D';
			callBasicFuncs(remainingLine,structureTree,'D',hashTable);
		}
		else if(strcmp(wordCase,"F")==0){
			
			startingLetter = 'F';
		}
		else{			//different letter
			deleteArray(structureTree);
			structureTree=NULL;
			if (line){
				free(line);
				line=NULL;
			}
			return 0;
		}
		
			
	}//found eof
	
	
	deleteArray(structureTree);
	structureTree=NULL;
	if (line){
		free(line);
		line=NULL;
	}
	
	if(startingLetter !='F'){
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

	//printf("WORD IS %s %ld\n", word, strlen(word));
	
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
	
}

char* getString(dataNode* node){
	char* returnWord = NULL;
	if(node->isDynamic){
		returnWord = malloc((strlen(node->dynamicWord)+1)*sizeof(char));
		strcpy(	returnWord,  node->dynamicWord);
		return returnWord;
	}	
	
	returnWord = malloc(node->noOfChars*sizeof(char));	
	//sprintf(returnWord,"%s",node->word);
	//printf("%s %ld\n", node->word, strlen(node->word));
	for(int i=0; i<node->noOfChars; i++){
		returnWord[i] = node->word[i];
	}
//	printf("WOOOORD : %s , chars:%d\n",node->word,node->noOfChars);
	return returnWord;	
}








