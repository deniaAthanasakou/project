#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "auxMethods.h"



int initialize(FILE* file, arrayOfStructs* structureTree){
	

	initializeArray(structureTree);

	char *line = NULL;
	size_t len = 0;
	char read;

	if (file == NULL)
		return 1;
	
	
	int insert=0;
	while ((read = getline(&line, &len, file)) != -1) {
	
		//printf("%s",line);
	
		char* ngram = strtok(line, "\n");
		
		
		insert = callBasicFuncs(ngram,structureTree,'A');
		
		if (!insert){
			fprintf( stderr, "%s\n","Insert was unsuccessful");	
			break;
		}
			
	}
	//found eof

	if (line){
		free(line);
		line=NULL;
	}
	
		
	if (!insert)
		return 0;	

	return 1;

}


int callBasicFuncs(char* ngram, arrayOfStructs* array, char query){

	arrayWords* arrayW = stringToArray(ngram);
	int noOfWords = arrayW->length;
	char** arrayOfWords = arrayW->words;
	if(query == 'A'){
		if(!insert_ngram(array, arrayOfWords,noOfWords)){
			deleteArrayOfWords(arrayOfWords,noOfWords);
			if(arrayOfWords!=NULL){
				free(arrayOfWords);
				arrayOfWords=NULL;
				free(arrayW);
				arrayW = NULL;
			}
			return 0;
		}	
	}
	else if(query == 'Q'){
		search_ngram(array, arrayOfWords,noOfWords);
	}
	else if(query == 'D'){
		if(!delete_ngram(array, arrayOfWords,noOfWords)){
			deleteArrayOfWords(arrayOfWords,noOfWords);
			if(arrayOfWords!=NULL){
				free(arrayOfWords);
				arrayOfWords=NULL;
				free(arrayW);
				arrayW = NULL;
			}
			return 0;
		}	
	}
	
	deleteArrayOfWords(arrayOfWords,noOfWords);
	if(arrayOfWords!=NULL){
		free(arrayOfWords);
		arrayOfWords=NULL;
		free(arrayW);
		arrayW = NULL;
	}

	return 1;
	
}



arrayWords* stringToArray(char* ngram){
	char* pch;
	char** arrayOfWords; 
	pch = strtok (ngram," ");
	int noOfWords=0;
	arrayOfWords = malloc(noOfWords * sizeof(char*));
	while (pch != NULL)
	{	
		noOfWords++;
		arrayOfWords = (char**)realloc(arrayOfWords, noOfWords * sizeof(char*));		
		arrayOfWords[noOfWords-1]=malloc((strlen(pch)+1)* sizeof(char));
		strcpy(arrayOfWords[noOfWords-1],pch);    //add pch into arrayOfWords
		pch = strtok (NULL, " ");
	}
	
	arrayWords* arrayW = malloc(sizeof(arrayWords));
	arrayW->length = noOfWords;
	arrayW->words = arrayOfWords;
	return arrayW;
}

void deleteArrayOfWords(char** array,int length){
	if(array!=NULL){
		//printf("dns\n");
		for(int i=0;i<length;i++){
			//printf("dddwe\n");
			if( array[i]!=NULL){
				//printf("array[i] %s length %d\n",array[i], length);
				//printf("dddwe\n");
				free(array[i]);
				//printf("we\n");
				array[i] = NULL;
			}
		}
		//printf("we\n");
		
	}

}

checkItemExists* binarySearch(arrayOfStructs* array_of_str, dataNode* item, int first, int last)	
{
	checkItemExists* check = malloc(sizeof(checkItemExists));

	check->exists=false;
	
	dataNode* array = array_of_str->array;
    if (last < first){
    	check->exists=false;
		if(array[first].word!=NULL){
			if(strcmp(item->word,array[first].word)>0){
    			check->position=first + 1;
    			//printf("1111 %d\n",check->position);
    			}
    		else{
    			check->position=first;
    			//printf("2222 %d\n",check->position);
    			}
		
		}else{
			
			check->position = first+1;
			//printf("43333 %d\n",check->position);
		
		}
    	return 	check;
    	
     }

 	int mid = (first+last)/2;

 	if(array[mid].word==NULL){			//to prosthesa egw dn kserw an einai swsto
 		check->exists=false;
 		check->position=-1;
		return check;
 	}
    if(strcmp(item->word ,array[mid].word)==0){
		check->position=mid;
		//printf("check->position=mid%d\n",check->position);
		check->exists=true;
        return check;
     }
 
    if(strcmp(item->word,array[mid].word)>0){
        return binarySearch(array_of_str, item, mid+1, last);
    }
    return binarySearch(array_of_str, item, first, mid-1);
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
		/*array_of_str->array[0].word = malloc( (strlen(itemForInsert->word)+1)*sizeof(char));
    	strcpy(array_of_str->array[0].word,itemForInsert->word);
    	array_of_str->array[0].isFinal=itemForInsert->isFinal;
    	array_of_str->array[0].nextWordArray=itemForInsert->nextWordArray;*/
		return retPosition;
	}
	free(retPosition);
	retPosition=NULL;
	i=lastElement;
    j = i - 1;

    // find location where selected sould be inseretd
    checkItemExists* getPosition = binarySearch(array_of_str, itemForInsert, 0, j);
   // printf("loc %d \n",getPosition->position);
    if(getPosition->exists==true){
    	return getPosition;
    }

    loc=getPosition->position;
    // Move all elements after location to create space
    //printf("loc %d j %d\n",loc,j);
    while (j >= loc)
    {
        array_of_str->array[j+1] = array_of_str->array[j];
        j--;
    }
   // printf("pos10 %d\n",array_of_str->position);
   // printf("%d\n",j+1);
    array_of_str->array[j+1] = *itemForInsert;
   // printf("pos11 %d\n",array_of_str->position);
   /* printf("ins a\n");
    array_of_str->array[j+1].word = realloc(array_of_str->array[j+1].word, (strlen(itemForInsert->word)+1)*sizeof(char));
    strcpy(array_of_str->array[j+1].word,itemForInsert->word);
    printf("ins b\n");
    array_of_str->array[j+1].isFinal=itemForInsert->isFinal;
    printf("ins c\n");
    array_of_str->array[j+1].nextWordArray=itemForInsert->nextWordArray;*/
    getPosition->position=j+1;
    
    return getPosition;
}

void deletionSort(arrayOfStructs* array_of_str,	int position, int lastElement){

	// delete item in position	'position'
	//printf("del sort %d\n",position);
	while (position < lastElement -1)
	{
	    array_of_str->array[position] = array_of_str->array[position+1];
	    position++;
	}
	
	array_of_str->position--;
     
}

void printFullArray(arrayOfStructs* array_of_str, int position){	//prints all layers

	arrayOfStructs* tempArray = array_of_str;
	
	if(tempArray != NULL){
		int lastElement = tempArray->position;
	
		for(int i=0; i < lastElement; i++){
			printf("'%s' is in position %d, ", tempArray->array[i].word, i);
			if(i==lastElement-1){				//print only once
				printf("\b\b: ");
				printArray(tempArray,(tempArray->position-1));
			}
			if( tempArray->array[i].nextWordArray!=NULL){
				printFullArray( tempArray->array[i].nextWordArray, tempArray->array[i].nextWordArray->position);
				
			}
			//else
				//printf("found null array\n");
		}
	}
	//else
		//printf("Null array_of_str\n");	
		
}

void printArray(arrayOfStructs* array_of_str, int position){		//prints layer

	printf("ELEMENTS ARE: [");
	for(int k=0; k<= position;k++){

		printf("%s ",  array_of_str->array[k].word);
		
		if(k!=position)
			printf(", ");
	}
	printf("]\n");
	
	
}

void printArrayFinalWords(arrayOfStructs* array_of_str, int position){

	printf("FINAL ELEMENTS ARE: [");
	for(int k=0; k<= position;k++){
		if(array_of_str->array[k].isFinal){
			printf("%s ",  array_of_str->array[k].word);
		
			if(k!=position)
				printf(", ");
		}
	}
	printf("]\n");
	
	
}



//insert from query file
int executeQueryFile(FILE* file,arrayOfStructs* structureTree){

	char *line = NULL;
	size_t len = 0;
	char read;

	if (file == NULL)
		return 1;
	
	
	int insert=0;
	int search = 0;
	int delete = 0;
	while ((read = getline(&line, &len, file)) != -1) {
	
		//printf("%s",line);
	
		char* ngram = strtok(line, "\n");
		
		//find first letter for each case
		char* wordCase = strtok(ngram," ");
		char* remainingLine = strtok(NULL,"");

		//printf("remaining:%s\n",remainingLine);
		if(strcmp(wordCase,"A")==0){	
			//printf("INSERT\n");
			insert = callBasicFuncs(remainingLine,structureTree,'A');
			if (!insert){
				fprintf( stderr, "%s\n","Insert was unsuccessful");	
				break;
			}
		}
		else if(strcmp(wordCase,"Q")==0){
			//printf("SEARCH\n");
			search = callBasicFuncs(remainingLine,structureTree,'Q');		//isws na nai void
			if (!search){
				fprintf( stderr, "%s\n","Search was unsuccessful");	
				break;
			}
		}
		else if(strcmp(wordCase,"D")==0){
		//	printf("DELETE\n");
			delete = callBasicFuncs(remainingLine,structureTree,'D');
			if (!delete){
				fprintf( stderr, "%s\n","Delete was unsuccessful");	
				break;
			}
		}
		else if(strcmp(wordCase,"F")==0){
			//printf("END\n");
		}
		else{
			printf("Error in query_file structure.\n");
		}
			
	}
	//found eof

	if (line){
		free(line);
		line=NULL;
	}
	
		
	if (!insert || !delete || !search)
		return 0;	

	return 1;

}









