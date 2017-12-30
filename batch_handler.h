#ifndef _BATCH_HANDLER_H_
#define _BATCH_HANDLER_H_

typedef struct instruction instruction;
typedef struct arrayOfInstructions arrayOfInstructions;

struct instruction{
	char type;	//Q, A or D
	char* ngram;
	int num;
	int numForQ;
};

struct arrayOfInstructions{
	int length;			//in the beggining is 10
	instruction* array;
	int position;		//position of first null element
};

arrayOfInstructions* initializeInstructionArray();
void doubleInstructionArray(arrayOfInstructions* arrayOfInstr);
void insertInstructionArray(arrayOfInstructions* arrayOfInstr, instruction* node);

void destroyInstructionArray(arrayOfInstructions* arrayOfInstr);

void initializeInstruction(instruction* instr);
void destroyInstruction(instruction* instr);

void printInstruction(instruction* instr);
void printInstructionArray(arrayOfInstructions* arrayOfInstr);

void rearrangeArray(arrayOfInstructions* arrayOfInstr);
void moveUp(instruction* array, int position, int lastElement);



#endif
