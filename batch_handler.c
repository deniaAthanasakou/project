#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "batch_handler.h"


arrayOfInstructions* initializeInstructionArray(){
	arrayOfInstructions* arrayOfInstr = malloc(sizeof(arrayOfInstructions));
	arrayOfInstr->length = 10;
	arrayOfInstr->array = malloc(arrayOfInstr->length*sizeof(instruction));
	arrayOfInstr->position = 0;
	for(int i = 0; i< arrayOfInstr->length; i++){
		initializeInstruction(&arrayOfInstr->array[i]);
	}
	return arrayOfInstr;
}

void initializeInstruction(instruction* instr){
	instr->type = 'A'; 	//default
	instr->ngram = NULL;
	instr->num = 0;
	instr->numForQ = -1;	//default
}

void destroyInstruction(instruction* instr){
	instr->type = 'A'; //default
	if(instr->ngram!=NULL){
		free(instr->ngram);
		instr->ngram = NULL;
	}
	instr->num = 0;
	instr->numForQ = -1; //default
}




void doubleInstructionArray(arrayOfInstructions* arrayOfInstr){
	int oldLength = arrayOfInstr->length;
	arrayOfInstr->length = 2 * oldLength;
	arrayOfInstr->array = realloc(arrayOfInstr->array,arrayOfInstr->length*sizeof(instruction));
	for(int i = oldLength; i< arrayOfInstr->length; i++){
		initializeInstruction(&arrayOfInstr->array[i]);
	}
}



void insertInstructionArray(arrayOfInstructions* arrayOfInstr, instruction* node){
	if(arrayOfInstr->position == arrayOfInstr->length){		//must double array
		doubleInstructionArray(arrayOfInstr);
	}
	arrayOfInstr->array[arrayOfInstr->position] = *node;
	
	arrayOfInstr->position++;

}

void moveUp(instruction* array, int position, int lastElement){
	int fullMoveSize = 0;
	if(position<lastElement){
		fullMoveSize = (lastElement -position)*sizeof(array[position+1]);
		memmove(&(array[position]),&(array[position+1]), fullMoveSize);
	}
}

void rearrangeArray(arrayOfInstructions* arrayOfInstr){
	int firstQNum = 0;
	int flag = 0;
	for(int i=0 ;i<arrayOfInstr->position; i++){
		if(i!=arrayOfInstr->position -1){
			if(arrayOfInstr->array[i].type == 'Q'){
				if(flag == 0){
					flag = 1;
					firstQNum = arrayOfInstr->array[i].num;
				}
				instruction node = arrayOfInstr->array[i];
				moveUp(arrayOfInstr->array, i, arrayOfInstr->position-1);
				arrayOfInstr->array[arrayOfInstr->position -1] = node;
				if(flag==1 && arrayOfInstr->array[i].num==firstQNum){
					break;
				}
				if(arrayOfInstr->array[i].type == 'Q'){
					i--;
				}
					
			}
		}
	}
}



void destroyInstructionArray(arrayOfInstructions* arrayOfInstr){
	for(int i = 0; i< arrayOfInstr->position; i++){
		destroyInstruction(&arrayOfInstr->array[i]);
	}
	free(arrayOfInstr->array);
	arrayOfInstr->array = NULL;
	free(arrayOfInstr);
	arrayOfInstr = NULL;
}

void printInstructionArray(arrayOfInstructions* arrayOfInstr){
	printf("PRINTING ARRAY OF INSTRUCTIONS\n");
	for(int i=0; i < arrayOfInstr->position; i++){
		printInstruction(&arrayOfInstr->array[i]);
	}
}

void printInstruction(instruction* instr){
	//printf("QUERY: '%c'\t NGRAM: '%s'\t NUMBER: '%d'",instr->type, instr->ngram, instr->num);
	//printf("QUERY: '%c'\t NUMBER: '%d'",instr->type, instr->num);
	if(instr->type=='Q'){
		printf("NUMBER: '%d'\t NUMBER OF QUERY '%d'",instr->num,instr->numForQ);
		printf("\n");
	}
		//printf("\t NUMBER OF QUERY '%d'",instr->numForQ);
	
	
}














