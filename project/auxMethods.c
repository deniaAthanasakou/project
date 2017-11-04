#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "auxMethods.h"
#include "struct.h"


void initialize(FILE* file, arrayOfStructs* structureTree){
	

	

	char *line = NULL;
	size_t len = 0;
	char read;

	if (file == NULL)
		return;
	while ((read = getline(&line, &len, file)) != -1) {
	
		//printf("%s",line);
	
		char* ngram = strtok(line, "\n");
		callBasicFuncs(ngram,structureTree,'A');
	}
	//found eof

	if (line){
		free(line);
		line=NULL;
	}
}


void callBasicFuncs(char* ngram, arrayOfStructs* array, char query){

	//printf("ngram is '%s' query '%c' \n",ngram, query);

	arrayWords* arrayW = stringToArray(ngram);
	int noOfWords = arrayW->length;
	char** arrayOfWords = arrayW->words;
	if(query == 'A'){
		insert_ngram(array, arrayOfWords,noOfWords);
	}
	else if(query == 'Q'){
		char* searchString = search_ngram(array, arrayOfWords,noOfWords);
		//printf("STRING IS '%s'\n",searchString);
		free(searchString);
		searchString=NULL;
	}
	else if(query == 'D'){
		//printf("before\n");
		delete_ngram(array, arrayOfWords,noOfWords);
	//	printf("after\n");
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

checkItemExists* binarySearch(arrayOfStructs* array_of_str, dataNode* item, int first, int last, checkItemExists* check)	
{
	if(check==NULL)
		check = malloc(sizeof(checkItemExists));

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
        return binarySearch(array_of_str, item, mid+1, last, check);
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
		array_of_str->array[0] = *itemForInsert;
		//printf("a\n");
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
    checkItemExists* getPosition = binarySearch(array_of_str, itemForInsert, 0, j,NULL);
   // printf("loc %d \n",getPosition->position);
    if(getPosition->exists==true){
   // printf("b\n");
    	return getPosition;
    }

	//double array
	if(array_of_str->position == array_of_str->length-1){
		//printf("length was doubled\n");
		doubleLength(array_of_str);
		//printf("length is %d\n",array_of_str->length);
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
    
   // printf("c\n");
    return getPosition;
}

void deletionSort(arrayOfStructs* array_of_str,	int position, int lastElement){

	// delete item in position	'position'
	//printf("del sort %d\n",position);
	//printf("last %d\n", lastElement);
	
	if(array_of_str->array[position].word!=NULL){				//an auta mpoun mesa sth while ginetai xalia h lexh
		free(array_of_str->array[position].word);
		array_of_str->array[position].word=NULL;
	}
	if(array_of_str->array[position].nextWordArray!=NULL){
		deleteArray(array_of_str->array[position].nextWordArray);
		//free( array_of_str->array[position].nextWordArray);
		array_of_str->array[position].nextWordArray=NULL;
	}
	

	while (position < lastElement -1)
	{
		
		
	    array_of_str->array[position]=array_of_str->array[position+1];
	    position++;
	}
	
	//printf("element in position %d is %s\n",position-1,array_of_str->array[position-1].word);
	//printf("element in position %d is %s\n",position,array_of_str->array[position].word);
	/*if(array_of_str->array[position].word!=NULL){
		free(array_of_str->array[position].word);
		array_of_str->array[position].word=NULL;
	}
	//deleteDataNode(&(array_of_str->array[position]));
	*/
	//
	//printf("ppppppooooooooooossssss %d\n",position);
	//printf(" before : %d\n",array_of_str->position);
	array_of_str->position--;
	//printf(" after : %d\n",array_of_str->position );
     
}

void printFullArray(arrayOfStructs* array_of_str, int position){	//prints all layers
	//printf("inside printfull\n");
	arrayOfStructs* tempArray = array_of_str;
	
	if(tempArray != NULL){
		int lastElement = tempArray->position;
	
		for(int i=0; i < lastElement; i++){
			printf("'%s' is in position %d, ", tempArray->array[i].word, i);
			if(i==lastElement-1){				//print only once
				//printf("kk\n");
				printf("\b\b: ");
				printArray(tempArray,(tempArray->position-1));
			}
			if( tempArray->array[i].nextWordArray!=NULL){
				//printf("ll\n");
				//printf("pos of next %d\n",tempArray->array[i].nextWordArray->position);
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
void executeQueryFile(FILE* file,arrayOfStructs* structureTree){

	char *line = NULL;
	size_t len = 0;
	char read;

	if (file == NULL)
		return;

	while ((read = getline(&line, &len, file)) != -1) {
	
		//printf("%s",line);
	
		char* ngram = strtok(line, "\n");
		
		//find first letter for each case
		char* wordCase = strtok(ngram," ");
		char* remainingLine = strtok(NULL,"");

		//printf("remaining:%s\n",remainingLine);
		if(strcmp(wordCase,"A")==0){	
			//printf("INSERT\n");
			callBasicFuncs(remainingLine,structureTree,'A');
		}
		else if(strcmp(wordCase,"Q")==0){
			//printf("SEARCH\n");
			callBasicFuncs(remainingLine,structureTree,'Q');		//isws na nai void
		}
		else if(strcmp(wordCase,"D")==0){
			callBasicFuncs(remainingLine,structureTree,'D');
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

}









