#ifndef _STACK_H_
#define _STACK_H_
#include <stdbool.h>


typedef struct stack stack;

struct stack{
	int* positionsToDelete;		//pinakas apo theseis pou prepei na diagrafoun
	int top;					//thesh tou teleutaiou stoixeiou;
	int length;					//megethos tou pinaka
};

void initializeStack(stack* myStack);
void doubleLengthOfStack(stack* myStack);

void push(stack* myStack, int item);
int pop(stack* myStack);
bool isEmpty(stack* myStack);
void displayStack(stack* myStack);
void deleteStack(stack* myStack);


#endif
