#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"
#include "func.h"
#include "hashTable.h"
#include "auxMethods.h"


int main(void){
	arrayOfStructs* array_of_str = malloc(sizeof(arrayOfStructs));
	initializeArray(array_of_str);
	HashTable* table = createLinearHash(4,4);
	char **words = malloc(1*sizeof(char*));
	words[0] = malloc((strlen("hello")+1)*sizeof(char));
	strcpy(words[0],"hello");
	insert_ngram(array_of_str, table, words, 1);
	words[0] = malloc((strlen("whatisthisplaceohmygod")+1)*sizeof(char));
	strcpy(words[0],"whatisthisplaceohmygod");
	insert_ngram(array_of_str, table, words, 1);
	
	int sizes[2];
	sizes[0] = 0;
	sizes[1] = 0;
	for(int i=0;i<2;i++){
		sizes[i] += sizeof(array_of_str->array[i].nextWordArray);
		sizes[i] += sizeof(array_of_str->array[i].word);
		sizes[i] += sizeof(array_of_str->array[i].isFinal);
		sizes[i] += sizeof(array_of_str->array[i].isDynamic);
		if(array_of_str->array[i].isDynamic){
			sizes[i] += strlen(array_of_str->array[i].dynamicWord);
		}
		else{
			sizes[i] += sizeof(array_of_str->array[i].dynamicWord);
		}
		sizes[i] += sizeof(array_of_str->array[i].noOfChars);
	}
	unsigned long sub1= (unsigned long) &array_of_str->array[2];
	unsigned long sub2= (unsigned long) &array_of_str->array[0];
	unsigned long sub= sub1-sub2;
	printf("FIRST : %d + SECOND: %d\n",sizes[0],sizes[1]);
	printf("addr sub : %ld\n",sub);
	//mhpos &(array_of_str->array[0]) -> array_of_str->array??
	memmove(&(array_of_str->array[2]),&(array_of_str->array[0]),sub);
	array_of_str->array[0] = array_of_str->array[2];
	printf("%s\n",array_of_str->array[2].dynamicWord);
	
	
	
	//-------------------
	/*
	char **wordArray = malloc(11*sizeof(char*));
	char *titles[10] = {"hello1","hello2","hello3","hello4","hello5","hello6","hello7","hello8","hello9","hello10"};
	int size=0;
	for(int i=0;i<10;i++){
		wordArray[i] = malloc((strlen(titles[i])+1)*sizeof(char));
		size += strlen(titles[i]);
		strcpy(wordArray[i],titles[i]);
	}
	
	
	memmove(&wordArray[10],&wordArray[9],strlen(wordArray[9]));
	printf("%s\n",wordArray[10]);*/
	return 0;
}