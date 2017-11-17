CC = gcc -g
FILES = AllTests.c CuTest.c CuTestStack.c CuTestAuxMethods.c CuTestFunc.c ../struct.c ../func.c ../auxMethods.c ../stack.c 
OBJECTS = AllTests.o CuTest.o CuTestStack.o CuTestAuxMethods.o CuTestFunc.o struct.o func.o auxMethods.o stack.o
OUT = test
HEADERS = CuTest.h CuTestStack.h CuTestAuxMethods.h CuTestFunc.h  ../struct.h ../func.h ../auxMethods.h ../stack.h

all: $(OBJECTS) $(HEADERS)
	$(CC) -o $(OUT) $(FILES)
	make clean

AllTests.o: AllTests.c
	$(CC) -c AllTests.c
	
CuTest.o: CuTest.c
	$(CC) -c CuTest.c	

CuTestStack.o: CuTestStack.c
	$(CC) -c CuTestStack.c
	
CuTestAuxMethods.o: CuTestAuxMethods.c
	$(CC) -c CuTestAuxMethods.c	
	
CuTestFunc.o: CuTestFunc.c
	$(CC) -c CuTestFunc.c		

struct.o: ../struct.c
	$(CC) -c ../struct.c

stack.o: ../stack.c
	$(CC) -c ../stack.c	
		
func.o: ../func.c
	$(CC) -c ../func.c 

auxMethods.o: ../auxMethods.c
	$(CC) -c ../auxMethods.c


clean:
	rm -f $(OBJECTS)

