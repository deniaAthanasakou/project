#ifndef _STACK_H_
#define _STACK_H_
#include <stdbool.h>


typedef struct stack stack;

struct stack{
	int* positionsToDelete;		//array of positions that must be deleted
	int top;					//position of last element
	int length;					//length of array
};

void initializeStack(stack* myStack);
void doubleLengthOfStack(stack* myStack);

void push(stack* myStack, int item);
int pop(stack* myStack);
bool isEmpty(stack* myStack);
void displayStack(stack* myStack);
void deleteStack(stack* myStack);


#endif
