#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

void initializeStack(stack* myStack){
	myStack->length=10;
	myStack->positionsToDelete = malloc(myStack->length*sizeof(int));
	myStack->top=-1;
}


void doubleLengthOfStack(stack* myStack){
	myStack->length*=2;
	myStack->positionsToDelete = (int*) realloc(myStack->positionsToDelete , myStack->length * sizeof(int));
	
}

void push(stack* myStack, int item){
    if (myStack->top == (myStack->length - 1))
    {
        printf ("prepei na diplasiastei h stoiva\n");
        doubleLengthOfStack(myStack);
    }
    myStack->top++;
    myStack->positionsToDelete[myStack->top]=item;
}

int pop(stack* myStack){
    if (isEmpty(myStack))
    {
        printf ("Stack is Empty\n");
        return 0;
    }
    myStack->top--;
    return 1;
}

bool isEmpty(stack* myStack){
	if(myStack->top==-1)
		return true;
	return false;	
}

void displayStack(stack* myStack){
    if (myStack->top == -1)
    {
        printf ("Stack is empty\n");
        return;
    }
    else
    {
        printf ("\n The status of the stack is \n[");
        for (int i = myStack->top; i >= 0; i--)
        {
            printf ("layer %d: %d",i, myStack->positionsToDelete[i]);
            if(i!=0)
            	printf(", ");
        }
    }
    printf ("]\n");
}
