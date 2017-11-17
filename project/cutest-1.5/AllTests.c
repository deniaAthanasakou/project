#include <stdio.h>

#include "CuTest.h"
#include "CuTestStack.h"
#include "CuTestAuxMethods.h"
#include "CuTestFunc.h"

CuSuite* CuGetSuite();
CuSuite* CuStringGetSuite();

void RunAllTests(void)
{
	CuString *output = CuStringNew();
	CuSuite* suite = CuSuiteNew();

	//CuSuiteAddSuite(suite, CuGetSuite());
	//CuSuiteAddSuite(suite, CuStringGetSuite());
	
	CuSuiteAddSuite(suite, StackGetSuite());
	CuSuiteAddSuite(suite, AuxMethodsGetSuite());
	CuSuiteAddSuite(suite, FuncGetSuite());

	CuSuiteRun(suite);
	CuSuiteSummary(suite, output);
	CuSuiteDetails(suite, output);
	printf("%s\n", output->buffer);
}

int main(void)
{
	RunAllTests();
}
