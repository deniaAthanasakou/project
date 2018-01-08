CC = gcc -g -pthread
FILES = AllTests.c CuTest.c CuTestStack.c CuTestStruct.c CuTestJobScheduler.c CuTestBatchHandler.c CuTestBloomFilter.c CuTestCompress.c CuTestTopK.c CuTestAuxMethods.c CuTestHashTable.c CuTestFunc.c ../struct.c ../func.c ../auxMethods.c ../stack.c ../bloomfilter.c ../hashTable.c ../murmur3/murmur3.c ../topK.c ../compress.c ../batch_handler.c ../JobScheduler.c
OBJECTS = AllTests.o CuTest.o CuTestStack.o CuTestStruct.o CuTestBatchHandler.o CuTestJobScheduler.o CuTestBloomFilter.o CuTestCompress.o CuTestTopK.o CuTestAuxMethods.o CuTestHashTable.o CuTestFunc.o struct.o func.o auxMethods.o stack.o bloomfilter.o hashTable.o murmur3.o topK.o compress.o batch_handler.o JobScheduler.o
OUT = test
HEADERS = CuTest.h CuTestStack.h CuTestStruct.h CuTestBatchHandler.h CuTestJobScheduler.h CuTestBloomFilter.h CuTestCompress.h CuTestTopK.h CuTestAuxMethods.h CuTestHashTable.h CuTestFunc.h ../struct.h ../func.h ../auxMethods.h ../stack.h ../bloomfilter.h ../hashTable.h ../murmur3/murmur3.h ../topK.c ../compress.h ../batch_handler.h ../JobScheduler.h

all: $(OBJECTS) $(HEADERS)
	$(CC) -o $(OUT) $(FILES) -lm
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

CuTestStruct.o: CuTestStruct.c
	$(CC) -c CuTestStruct.c
	
CuTestBloomFilter.o: CuTestBloomFilter.c
	$(CC) -c CuTestBloomFilter.c

CuTestCompress.o: CuTestCompress.c
	$(CC) -c CuTestCompress.c

CuTestTopK.o: CuTestTopK.c
	$(CC) -c CuTestTopK.c
	
CuTestHashTable.o: CuTestHashTable.c
	$(CC) -c CuTestHashTable.c	

CuTestJobScheduler.o: CuTestJobScheduler.c
	$(CC) -c CuTestJobScheduler.c
	
CuTestBatchHandler.o: CuTestBatchHandler.c
	$(CC) -c CuTestBatchHandler.c
	
struct.o: ../struct.c
	$(CC) -c ../struct.c

stack.o: ../stack.c
	$(CC) -c ../stack.c	
		
func.o: ../func.c
	$(CC) -c ../func.c 

auxMethods.o: ../auxMethods.c
	$(CC) -c ../auxMethods.c
	
bloomfilter.o: ../bloomfilter.c
	$(CC) -c ../bloomfilter.c
	
hashTable.o: ../hashTable.c
	$(CC) -c ../hashTable.c	
	
murmur3.o: ../murmur3/murmur3.c
	$(CC) -c ../murmur3/murmur3.c

topK.o: ../topK.c
	$(CC) -c ../topK.c
	
compress.o: ../compress.c
	$(CC) -c ../compress.c		

batch_handler.o: ../batch_handler.c
	$(CC) -c ../batch_handler.c		

JobScheduler.o: ../JobScheduler.c
	$(CC) -c ../JobScheduler.c
	


clean:
	rm -f $(OBJECTS)

