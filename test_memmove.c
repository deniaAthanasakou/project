#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"
#include "func.h"
#include "hashTable.h"
#include "auxMethods.h"


int main(void){

	//douleuei mia xara
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
	printf("correct\n");
	int sizes[2];
	sizes[0] = 0;
	sizes[1] = 0;
	int sizeOfStruct = 0;
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
		
		sizeOfStruct = sizeof(array_of_str->array[i]);
		
		printf("size is '%d' sizeofstruct is '%d'\n",sizes[i],sizeOfStruct);
		
	}
	unsigned long sub1= (unsigned long) &array_of_str->array[2];
	unsigned long sub2= (unsigned long) &array_of_str->array[0];
	unsigned long sub= sub1-sub2;
	printf("FIRST : %d + SECOND: %d\n",sizes[0],sizes[1]);
	printf("addr sub : %ld\n",sub);
	//mhpos &(array_of_str->array[0]) -> array_of_str->array??
	//memmove(&(array_of_str->array[2]),&(array_of_str->array[0]),sub);
	memmove(&(array_of_str->array[2]),&(array_of_str->array[0]),sizes[0]+sizes[1]);		//vale sto array[2]- array[3] ta  array[0]- array[1] 
	printf("after memmove\n");
	array_of_str->array[0] = array_of_str->array[3];
	printf("array_of_str->array[0] = %s\n",array_of_str->array[0].dynamicWord);
	printf("array_of_str->array[2] = %s\n",array_of_str->array[2].word);
	printf("array_of_str->array[3] %s\n",array_of_str->array[3].dynamicWord);
	
	
	
	//-------------------	douleuei komple
	//PROSOXH thelei sizeof gia to size kai oxi strlen
	printf("String Example\n");
	
	char **wordArray = malloc(30*sizeof(char*));
	char *titles[10] = {"hello1","hello2","hello3","hello4","hello5","hello6","hello7","hello8","hello9","hello10"};
	int size=0;
	for(int i=0;i<10;i++){
		//wordArray[i] = malloc((strlen(titles[i])+1)*sizeof(char));
		size += sizeof(titles[i]);
		//strcpy(wordArray[i],titles[i]);
	}
	printf("memmove with size %d\n",size);
	//memmove(&wordArray[10],&wordArray[9],strlen(wordArray[9]));		//vale sto wordArray[10] = "hello10"
	memmove(&wordArray[0],&titles[0],size);		//vale sto wordArray[0] - wordArray[9] ta  titles[0] - titles[9]
	memmove(&wordArray[10],&titles[0],size);		//vale sto wordArray[10] - wordArray[19] ta  titles[0] - titles[9]
	memmove(&wordArray[20],&wordArray[10],size);		//vale sto wordArray[20] - wordArray[29] ta  wordArray[10] - wordArray[19]
	//printf("%s\n",wordArray[10]);
	for(int i=0; i<30; i++){
		printf("i = %d %s\n",i,wordArray[i]);
	}
	return 0;
}
