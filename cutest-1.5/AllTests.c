#include <stdio.h>
#include <stdlib.h>
#include "CuTest.h"
#include "CuTestStack.h"
#include "CuTestAuxMethods.h"
#include "CuTestFunc.h"
#include "CuTestStruct.h"
#include "CuTestTopK.h"

CuSuite* CuGetSuite();
CuSuite* CuStringGetSuite();


void RunAllTests(void)
{
	CuString *output = CuStringNew();
	CuSuite* suite = CuSuiteNew();

	//CuSuiteAddSuite(suite, CuGetSuite());
	//CuSuiteAddSuite(suite, CuStringGetSuite());
	
	CuSuite* stackSuite =  StackGetSuite();
	CuSuiteAddSuite(suite,stackSuite);
	free(stackSuite);
	stackSuite=NULL;
	
	CuSuite* auxMethodsSuite =  AuxMethodsGetSuite();
	CuSuiteAddSuite(suite, auxMethodsSuite);
	free(auxMethodsSuite);
	auxMethodsSuite=NULL;
	
	/*CuSuite* funcSuite =  FuncGetSuite();
	CuSuiteAddSuite(suite, funcSuite);
	free(funcSuite);
	funcSuite=NULL;
*/	

	//topK
	//compress
	//bloomFilter	
	//hashTable
	
	CuSuite* topKSuite =  TopKGetSuite();
	CuSuiteAddSuite(suite, topKSuite);
	free(topKSuite);
	topKSuite=NULL;
	
	CuSuite* structSuite =  StructGetSuite();
	CuSuiteAddSuite(suite, structSuite);
	free(structSuite);
	structSuite=NULL;

	CuSuiteRun(suite);
	CuSuiteSummary(suite, output);
	CuSuiteDetails(suite, output);
	printf("%s\n", output->buffer);
	

	CuStringDelete(output);
	
	CuSuiteDelete(suite);
	
}



int main(void)
{
	RunAllTests();
}
