#ifndef CU_BATCH_HANDLER_H
#define CU_BATCH_HANDLER_H
#include "CuTest.h"
#include "../batch_handler.h"

void TestInitializeInstructionArray(CuTest *tc);
void TestDoubleInstructionArray(CuTest *tc);
void TestInsertInstructionArray(CuTest *tc);
void TestInitializeInstruction(CuTest *tc);
void TestDestroyInstruction(CuTest *tc);
void TestDestroyInstructionArray(CuTest *tc);
void TestRearrange_MoveUpArray(CuTest *tc);
void TestMoveUp(CuTest *tc);
void destroyInstruction(instruction* instr);

CuSuite* BatchHandlerGetSuite();

#endif
