#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "auxMethods.h"
#include "struct.h"


int initialize(FILE* file, arrayOfStructs* structureTree, HashTable* hashTable){

	char *line = NULL;
	size_t len = 0;
	int read;
	int counter = 0;
	int returnValue = 1;		//default
	if (file == NULL)
		return returnValue;
	while ((read = getline(&line, &len, file)) != -1) {	
		char* ngram = strtok(line, "\n");
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
		else
			callBasicFuncs(ngram,structureTree,'A',hashTable);
		counter++;
	}
	//found eof

	if (line){
		free(line);
		line=NULL;
	}
	return returnValue;
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
		//array_of_str->array[0] = *itemForInsert;
		memmove(&(array_of_str->array[0]), itemForInsert, sizeof(*itemForInsert));
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
	//int startingPoint = j-1;			//an vroume auto ti prepei na einai tha doulepsei komple
	int startingPoint = getPosition->position;	
	if( j>=loc){
		while (j >= loc)
		{
			moveSize+=sizeof(array_of_str->array[j]);
			j--;
			
		}
		memmove(&(array_of_str->array[startingPoint+1]), &(array_of_str->array[startingPoint]), moveSize);
	}
	//array_of_str->array[j+1] = *itemForInsert;
	memmove(&(array_of_str->array[j+1]), itemForInsert, sizeof(*itemForInsert));
    getPosition->position=j+1;
    
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
	
	printf("/////////////////////////////////////////////inserting word '%s'\n", getString(itemForInsert));
	dataNode* array = malloc((bucket->noOfElements)*sizeof(dataNode));
	Bucket* tempBucket = bucket;
	int arrayCounter=0;
	int cellCounter=0;
	while(tempBucket!=NULL){		
		if(cellCounter == tempBucket->length){
		printf("next temp\n");
			tempBucket=tempBucket->nextBucket;
			cellCounter = 0;
		}
		if(tempBucket == bucket && tempBucket->position == 0) {
			printf("a\n");
			break;
		}
		int position = tempBucket->position;
		if(cellCounter >= position) {
			break;
		}	
			
		array[arrayCounter] = tempBucket->cells[cellCounter];
		arrayCounter++;
		cellCounter++;
	}
	printf("printing array arrayCounter %d\n", arrayCounter);
	for(int i =0; i <arrayCounter; i++){
		printf("word '%s'\n", getString(&(array[i])));
	}
	
	printf("after array\n");


    int i, loc, j;
    
    checkItemExists* retPosition=malloc(sizeof(checkItemExists));
    retPosition->position=0;	//insert in first elements
    retPosition->exists=false;
    
	if(lastElement==0){
		memmove(&(bucket->cells[0]),itemForInsert, sizeof(*itemForInsert));
		bucket->position++;
		return retPosition;
	}
	free(retPosition);
	retPosition=NULL;
    j = lastElement - 1;

    // find location where selected sould be inseretd
    checkItemExists* getPosition = binarySearch2(array, itemForInsert, 0, j,NULL);
    if(getPosition->exists==true){
    	return getPosition;
    }
    
    
    
    loc=getPosition->position;
    array = realloc(array,(bucket->noOfElements +1 )*sizeof(dataNode));
	int moveSize=0;
	int startingPoint = getPosition->position;
	while (j >= loc)
	{
	   moveSize+=sizeof(array[j]);
	    j--;
	}
	memmove(&array[startingPoint+1],&array[startingPoint],moveSize);
	memmove(&(array[j+1]),itemForInsert,sizeof(*itemForInsert));
	moveSize+=sizeof(array[j+1]);
	printf("printing new array\n");
	for(int i =0; i <arrayCounter+1; i++){
		printf("word '%s'\n", getString(&(array[i])));
	}
	
	printf("after new array\n");
	
	int noOfExtraBucketsToBeUsed = ((bucket->noOfElements ) / bucket->length);
	//printf("noOfBucketsToBeUsed is %d\n", noOfExtraBucketsToBeUsed);
	
	//create chunks

	Bucket* bucketForInsert = bucket;
	
	int* sizes = malloc(0*sizeof(int));
	for(int i=0; i<=bucket->noOfElements +1; i++){	//store chunk size
		sizes = realloc(sizes,(i+1)*sizeof(int));
		sizes[i] = sizeof(array[i]);
	}
	int chunkCounter = 0;
	int sizesCounter = 0;
	for(int i=0; i<=noOfExtraBucketsToBeUsed;i++){	//memmove inside bucket
		int sizeOfChunk = 0;
		int j =0;
		while(j<bucket->length && j<bucket->noOfElements +1){
			sizeOfChunk+=sizes[sizesCounter];
			sizesCounter++;
			j++;
		}

		memmove(&(bucketForInsert->cells[0]),&array[chunkCounter],sizeOfChunk);
		printf("bucket %d \n",i);
		printf("word in cell 0 '%s'\n", getString(&(bucketForInsert->cells[0])));
		printf("word in cell 1 '%s'\n",getString(&(bucketForInsert->cells[1])));
		printf("word in cell 2 '%s'\n",getString(&(bucketForInsert->cells[2])));
		printf("word in cell 3 '%s'\n",getString(&(bucketForInsert->cells[3])));
		
		if(i==noOfExtraBucketsToBeUsed)
			bucketForInsert->position++;
		
		
		if(bucketForInsert->position==bucketForInsert->length && bucketForInsert->nextBucket==NULL){	//overflow
			printf("inside overflow\n");
			bucketForInsert->nextBucket = malloc(sizeof(Bucket));
			initializeBucket(bucketForInsert->nextBucket,bucketForInsert->length,bucketForInsert->noOfElements);
			printf("after init\n");
		}
		
		bucketForInsert = bucketForInsert->nextBucket;
		
		chunkCounter+=bucket->length;
		
	}
	//printf("before position++\n");
	
	//printf("after position++\n");
	//printf("word in bucket 0 '%s'\n",bucket->cells[0].word);
	
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
	int moveSize = 0;
	int startingPoint = position;
	while (position < lastElement -1)
	{
	    //array_of_str->array[position]=array_of_str->array[position+1];
	    moveSize+=sizeof(array_of_str->array[position+1]);
	    position++;
	}
	memmove(&(array_of_str->array[startingPoint]),&(array_of_str->array[startingPoint+1]), moveSize);
	array_of_str->position--;
     
}

void printFullArray(arrayOfStructs* array_of_str, int position){	//prints all layers
	arrayOfStructs* tempArray = array_of_str;
	
	if(tempArray != NULL){
		int lastElement = position;
		//printf("lastElement is %d\n",lastElement);
	
		for(int i=0; i < lastElement; i++){
			char* word = getString(&( tempArray->array[i]));
			printf("'%s' is in position %d, ", word, i);
			free(word);
			word=NULL;
			if(i==lastElement-1){				//print only once
				printf("\b\b: ");
				printArray(tempArray,(position-1));
			}
			if( tempArray->array[i].nextWordArray!=NULL){
				printFullArray( tempArray->array[i].nextWordArray, position);
				
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
int executeQueryFile(FILE* file ,arrayOfStructs* structureTree , HashTable* hashTable, int staticDynamic){

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
			if(staticDynamic==0){	//STATIC
				printf("Error with init file! Add is not supported in Static version.\n");
				deleteArray(structureTree);
				structureTree=NULL;
				if (line){
					free(line);
					line=NULL;
				}
				exit(1);
			}
			callBasicFuncs(remainingLine,structureTree,'A',hashTable);
		}
		else if(strcmp(wordCase,"Q")==0){
			startingLetter = 'Q';
			callBasicFuncs(remainingLine,structureTree,'Q',hashTable);		
		}
		else if(strcmp(wordCase,"D")==0){
			startingLetter = 'D';
			if(staticDynamic==0){	//STATIC
				printf("Error with init file! Delete is not supported in Static version.\n");
				deleteArray(structureTree);
				structureTree=NULL;
				if (line){
					free(line);
					line=NULL;
				}
				exit(1);
			}
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
	//printf("start\n");
	if(node->isDynamic){
	//	printf("is dynamic\n");
		returnWord = malloc((strlen(node->dynamicWord)+1)*sizeof(char));
		strcpy(	returnWord,  node->dynamicWord);
		return returnWord;
	}	
	//printf("is not dynamic\n");
	//printf("node->noOfChars %d\n",node->noOfChars);
	returnWord = malloc((node->noOfChars)*sizeof(char));
	//printf("after malloc\n");	
	//sprintf(returnWord,"%s",node->word);
	//printf("%s %ld\n", node->word, strlen(node->word));
	for(int i=0; i<node->noOfChars; i++){
		returnWord[i] = node->word[i];
	}
//	printf("WOOOORD : %s , chars:%d\n",node->word,node->noOfChars);
	return returnWord;	
}








