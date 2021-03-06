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
			callBasicFuncs(NULL,ngram,'A',hashTable,NULL,NULL,returnValue, 0,NULL);
		}
		counter++;
	}

	if (line){
		free(line);
		line=NULL;
	}
	return returnValue;
}


arrayWords* callBasicFuncs(JobScheduler* sch,char* ngram, char query , HashTable* hashTable, BloomFilter* topFilter, topKArray *topArray, int isDynamic, int instrNum,instruction* instr){

	arrayWords* arrayW = stringToArray(ngram);
	int noOfWords = arrayW->length;
	char** arrayOfWords = arrayW->words;
	if(query == 'A'){
		insert_ngram(hashTable, arrayOfWords,noOfWords, instrNum);
		free(arrayW->words);
		arrayW->words = NULL;
		free(arrayW);
		arrayW = NULL;
		return NULL;
	}
	else if(query == 'Q'){
		
		int* noOfWordsPtr = &(arrayW->length);
		void** p = malloc(6*sizeof(void*));
		p[0] = (void*)hashTable;
		p[1] = (void*)arrayW->words;
		p[2] = (void*)noOfWordsPtr;
		p[3] = (void*)topFilter;
		p[4] = (void*)topArray;
		p[5] = (void*)&instr->num;
		if(isDynamic){
			Job* j = initializeJob((void*)search_ngram,p);
			submitJob(sch,j);	
			return arrayW;
		}
		
		else{
			Job* j = initializeJob((void*)search_ngram_StaticVersion,p);
			submitJob(sch,j);	
			return arrayW;
		}
	}
	else if(query == 'D'){
		delete_ngram(hashTable, arrayOfWords,noOfWords,instrNum);
		free(arrayW->words);
		arrayW->words = NULL;
		free(arrayW);
		arrayW = NULL;
		return NULL;
	}
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
	if(tempNode->dynamicWord!=NULL){
		node->dynamicWord = malloc(tempNode->noOfChars*sizeof(char));
		strcpy(node->dynamicWord , tempNode->dynamicWord);
	}
	else
		node->dynamicWord=NULL;	
	node->noOfChars = tempNode->noOfChars;
	for(int i=0; i<tempNode->noOfChars; i++){
		node->word[i] = tempNode->word[i];
	}

	node->staticArray = tempNode->staticArray;
	node->staticArrayLength = tempNode->staticArrayLength;
	
}


checkItemExists* binarySearch(arrayOfStructs* array_of_str, dataNode* item, int first, int last, checkItemExists* check, bool add)	
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

	if(array[mid].isEmpty)
	{
		check->exists=false;
 		check->position=-1;
		return check;
	}

 	
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

		if(add==true &&array[mid].finalSinceAdditionVersion==-1){
			array[mid].finalSinceAdditionVersion=item->finalSinceAdditionVersion;
		}
			
		if(add==true && array[mid].deletionVersion>=array[mid].additionVersion && item->additionVersion>array[mid].deletionVersion){
			array[mid].additionVersion=item->additionVersion;
			array[mid].deletionVersion = -1;
		}
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
	    return binarySearch(array_of_str, item, mid+1, last, check, add);
	}
	
	if(wordMidMalloc!=NULL){
		free(wordMidMalloc);
		wordMidMalloc = NULL;
	}
	return binarySearch(array_of_str, item, first, mid-1, check, add);


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
    checkItemExists* getPosition = binarySearch(array_of_str, itemForInsert, 0, j,NULL, true);
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



 

checkItemExists* binarySearchBucket(dataNode* array, dataNode* item, int first, int last, int trueLastElement, checkItemExists* check, bool add)	
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
		
		if(add==true && array[mid].finalSinceAdditionVersion==-1){
			array[mid].finalSinceAdditionVersion=item->finalSinceAdditionVersion;
		}
		
		if(add==true &&  array[mid].deletionVersion>=array[mid].additionVersion && item->additionVersion>array[mid].deletionVersion){
			array[mid].additionVersion=item->additionVersion;
			
			array[mid].deletionVersion = -1;
		}
		check->position=mid;
		check->exists=true;
	    return check;
	 }
 
	if(strcmp(wordItem,wordMid)>0){
	    return binarySearchBucket(array, item, mid+1, last,trueLastElement, check,add);
	}
	return binarySearchBucket(array, item, first, mid-1,trueLastElement, check,add);
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
    checkItemExists* getPosition = binarySearchBucket(bucket->cells, itemForInsert, 0, j,bucket->position, NULL,true);
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
	initializeDataNode(&array_of_str->array[position]);
			
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
		printf("%s (add_version %d, del_version %d)",word, array_of_str->array[k].additionVersion,array_of_str->array[k].deletionVersion);
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
	
	//initialize JobScheduler
	JobScheduler *sch = NULL;
		
	char endingLetter = 'F';			//initialization	
	int counter=-1;
	BloomFilter* topFilter = NULL;
	topKArray* topArray = NULL;
	int counterForBatch = 0;
	int realCounterForBatch = 0;
	arrayOfInstructions* arrayOfInstr = NULL;
	instruction* node = NULL;
	arrayOfInstrStatic* arrayOfInstrStatic = NULL;
	instructionStatic* nodeStatic = NULL;
	arrayWords** arrayW = NULL;
	int numForQuery = 0;
	int queries = 0;
	while ((read = getline(&line, &len, file)) != -1) {
		char* ngram = strtok(line, "\n");
		if(ngram==NULL){
			continue;
		}
		counter++;
		if(counter == 0){		 
			topFilter = initializeFilter(5);		//initialize bloomFilter here
			topArray = initializeTopKArray();		//initialize topKArray
			sch = initializeScheduler(1);
		}
		
		
		//find first letter for each case
		char* wordCase = strtok(ngram," \t");
		char* remainingLine = strtok(NULL,"");
		if(counterForBatch == 0 && staticDynamic==1){
			arrayOfInstr = initializeInstructionArray();
		}

		if(counterForBatch == 0 && staticDynamic==0){
			arrayOfInstrStatic = initializeInstrStaticArr();
		}
		
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
				
				destroyScheduler(&sch);
				exit(1);
			}
			else{	//DYNAMIC
				
				if(remainingLine!=NULL){
					node = malloc(sizeof(instruction));
					node->type = endingLetter;
					node->ngram = malloc(sizeof(char)*(strlen(remainingLine)+1));
					strcpy(node->ngram,remainingLine);
					node->num = realCounterForBatch;
					insertInstructionArray(arrayOfInstr, node);
			
					free(node);
					node = NULL;
				}

				
			}
		
		}
		else if(strcmp(wordCase,"Q")==0){
			endingLetter = 'Q';
			if(staticDynamic==1){	//DYNAMIC
				node = malloc(sizeof(instruction));
				node->type = endingLetter;
				if(remainingLine!=NULL){
					node->ngram = malloc(sizeof(char)*(strlen(remainingLine)+1));
					strcpy(node->ngram,remainingLine);
				}
				else
					node->ngram = NULL;
				node->num = realCounterForBatch;
				node->numForQ = numForQuery;
				insertInstructionArray(arrayOfInstr, node);
		
				free(node);
				node = NULL;
				numForQuery++;
				queries++;
			}
			else{	//STATIC
				nodeStatic = malloc(sizeof(instructionStatic));
				nodeStatic->type = endingLetter;
				if(remainingLine!=NULL){
					nodeStatic->ngram = malloc(sizeof(char)*(strlen(remainingLine)+1));
					strcpy(nodeStatic->ngram,remainingLine);
				}
				else
					nodeStatic->ngram = NULL;
				nodeStatic->num = numForQuery;
				insertInstrStaticArray(arrayOfInstrStatic, nodeStatic);
		
				free(nodeStatic);
				nodeStatic = NULL;
				numForQuery++;
			}	
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
				
				destroyScheduler(&sch);
				
				exit(1);
			}
			else{	//DYNAMIC
				if(remainingLine!=NULL){
					node = malloc(sizeof(instruction));
					node->type = endingLetter;
					node->ngram = malloc(sizeof(char)*(strlen(remainingLine)+1));
					strcpy(node->ngram,remainingLine);
					node->num = realCounterForBatch;
					insertInstructionArray(arrayOfInstr, node);
			
					free(node);
					node = NULL;
				}
			}
		}
		else if(strcmp(wordCase,"F")==0){
			endingLetter = 'F';
			counter = -1;
			counterForBatch = -1;
			if(staticDynamic==1){	//DYNAMIC
				rearrangeArray(arrayOfInstr);
				arrayW = malloc(queries*sizeof(arrayWords*));
				
				executeDynamicArray(sch,arrayOfInstr,hashTable, topFilter, topArray,arrayW);
				
				sch->submittedAll = 1;	
				waitAllTasksFinish(sch);
				sch->submittedAll = 0;
				
				for(int i = 0; i<queries; i++){
					if(arrayW[i]->words!=NULL){
						free(arrayW[i]->words);
						arrayW[i]->words = NULL;
					}
					if(arrayW[i]!=NULL){
						free(arrayW[i]);
						arrayW[i] = NULL;
					}
				}
				destroyInstructionArray(arrayOfInstr);
				
				free(arrayW);
				arrayW = NULL;
				
				restructHashTable(hashTable);
				queries = 0;
			}else{
				arrayW = malloc(arrayOfInstrStatic->position*sizeof(arrayWords*));
				for(int i = 0; i<arrayOfInstrStatic->position; i++){
					arrayW[i] = callBasicFuncs(sch,arrayOfInstrStatic->array[i].ngram,'Q',hashTable,topFilter,topArray,0, 0,NULL);
				}
				
				
				sch->submittedAll = 1;	
				waitAllTasksFinish(sch);
				sch->submittedAll = 0;
				
				for(int i = 0; i<arrayOfInstrStatic->position; i++){
					if(arrayW[i]->words!=NULL){
						free(arrayW[i]->words);
						arrayW[i]->words = NULL;
					}
					if(arrayW[i]!=NULL){
						free(arrayW[i]);
						arrayW[i] = NULL;
					}
				}
				destroyInstrStaticArray(arrayOfInstrStatic);
				free(arrayW);
				arrayW = NULL;
			}
			
			//get top-k
			if(remainingLine!=NULL){
				int topK = atoi(remainingLine);		
				if(topK <= topArray->positionInsertion){
					HeapSort(topArray->array, topArray->positionInsertion, topK);	//sort based on integers	
					//print topK
					printTopK(topArray,topK);
				}
				
			}
			
			//free bloomFilter
			freeFilter(topFilter);
			//free array
			destroyTopArray(topArray);
			
			destroyScheduler(&sch);
			
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
			
			destroyScheduler(&sch);
			if(staticDynamic==1)
				destroyInstructionArray(arrayOfInstr);
			
			return 0;
		}
		
		counterForBatch++;
		realCounterForBatch++;
		
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
		
		destroyScheduler(&sch);
		if(staticDynamic==1)
			destroyInstructionArray(arrayOfInstr);
	
		return 0;
	}

	return 1;

}

void executeDynamicArray(JobScheduler *sch,arrayOfInstructions* arrayOfInstr, HashTable* hashTable, BloomFilter* topFilter, topKArray* topArray,arrayWords** arrayW){
	int counter=0;
	for(int i = 0; i<arrayOfInstr->position; i++){
		if(arrayOfInstr->array[i].type =='A'){
			callBasicFuncs(NULL,arrayOfInstr->array[i].ngram,'A',hashTable, NULL, NULL, 1, arrayOfInstr->array[i].num,NULL);
		}
		else if(arrayOfInstr->array[i].type =='D'){
			callBasicFuncs(NULL,arrayOfInstr->array[i].ngram,'D',hashTable, NULL, NULL, 1,arrayOfInstr->array[i].num,NULL);
		}
		else if(arrayOfInstr->array[i].type =='Q'){
			arrayW[counter] = callBasicFuncs(sch,arrayOfInstr->array[i].ngram,'Q',hashTable,topFilter,topArray,1, arrayOfInstr->array[i].num,&arrayOfInstr->array[i]);
			counter++;
		}
	}
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

bool hasChildren(dataNode* lookupElement, int deletionNum){
	arrayOfStructs* tempArray = lookupElement->nextWordArray;
	if(tempArray==NULL){
		return false;	
	}
	for(int i=0; i<	tempArray->position; i++){
		if(tempArray->array[i].deletionVersion!=deletionNum)
			return true;	
	}

	return false;
}







