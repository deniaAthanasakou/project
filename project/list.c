#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

int insert_ngram(char* ngram){
	printf ("===================================INSERT N GRAM=====================================\n");
	char* pch;
	char** arrayOfWords; 
	printf ("Splitting string %s! into tokens:\n",ngram);
	pch = strtok (ngram," ");
	int noOfWords=0;
	arrayOfWords = malloc(noOfWords * sizeof(char*));
	while (pch != NULL)
	{	
		noOfWords++;
		printf ("!%s! noofwords = %d\n",pch,noOfWords);
		arrayOfWords = (char**)realloc(arrayOfWords, noOfWords * sizeof(char*));		
		arrayOfWords[noOfWords]=malloc(strlen(pch)* sizeof(char));
		
		printf ("size of malloc %ld\n",strlen(pch)* sizeof(char));
		
		//add pch into arrayOfWords
		strcpy(arrayOfWords[noOfWords],pch);
		printf ("inside array !%s!\n",arrayOfWords[noOfWords]);
		
		
		pch = strtok (NULL, " ");
	
		
	}
	
	
	
	
	//for each word create node with sl=null(for now)
	/*for (int i=0; i<noOfWords; i++){ //free malloc of word
		listNode* node= malloc(sizeof(listNode));
		bool isFinal=false;
		if(i==noOfWords-1)		//last word
			isFinal=true;
		setNode(&node,NULL,getNextNodeSomehow, arrayOfWords[i],isFinal)
	}*/
	
	
	/*for (int i=0; i<noOfWords; i++){ //free malloc of word
		free(arrayOfWords[i]);
	}
	//free malloc of ngram
	free(arrayOfWords);
	*/


	return 1;
}

void setNode(listNode* this, listNode* sl,listNode* nw,char* data, bool f){
	this->sameLayer = sl;
	this->nextWord = nw;
	strcpy(this->word, data);
	this->isFinal = f;
}

