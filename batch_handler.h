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

typedef struct instructionStatic{
	char type;
	char* ngram;
	int num;
}instructionStatic;

typedef struct arrayOfInstrStatic{
	int length;
	instructionStatic* array;
	int position;
}arrayOfInstrStatic;

struct arrayOfInstructions{
	int length;			//in the beggining is 10
	instruction* array;
	int position;		//position of first null element
};

//dynamic
arrayOfInstructions* initializeInstructionArray();
void doubleInstructionArray(arrayOfInstructions* arrayOfInstr);
void insertInstructionArray(arrayOfInstructions* arrayOfInstr, instruction* node);

void destroyInstructionArray(arrayOfInstructions* arrayOfInstr);

void initializeInstruction(instruction* instr);

void printInstruction(instruction* instr);
void printInstructionArray(arrayOfInstructions* arrayOfInstr);

void rearrangeArray(arrayOfInstructions* arrayOfInstr);
void moveUp(instruction* array, int position, int lastElement);

//static
arrayOfInstrStatic* initializeInstrStaticArr();
void initializeInstrStatic(instructionStatic* instr);

void doubleInstrStaticArray(arrayOfInstrStatic* arrayOfInstr);
void insertInstrStaticArray(arrayOfInstrStatic* arrayOfInstr, instructionStatic* node);
void destroyInstructionStatic(instructionStatic* instr);

void destroyInstrStaticArray(arrayOfInstrStatic* arrayOfInstr);


#endif
