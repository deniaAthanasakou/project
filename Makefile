CC = gcc -g -pthread
FILES = main.c struct.c func.c auxMethods.c stack.c bloomfilter.c hashTable.c ./murmur3/murmur3.c topK.c compress.c batch_handler.c JobScheduler.c
OBJECTS = main.o struct.o func.o auxMethods.o stack.o bloomfilter.o hashTable.o murmur3.o topK.o compress.o batch_handler.o JobScheduler.o
OUT = ngrams
HEADERS = struct.h func.h auxMethods.h stack.h bloomfilter.h hashTable.h ./murmur3/murmur3.h topK.h compress.h batch_handler.h JobScheduler.h


all: $(OBJECTS) $(HEADERS)
	$(CC) -o $(OUT) $(FILES) -lm
	make clean
	
main.o: main.c
	$(CC) -c main.c

struct.o: struct.c
	$(CC) -c struct.c

stack.o: stack.c
	$(CC) -c stack.c	
		
func.o: func.c
	$(CC) -c func.c 

auxMethods.o: auxMethods.c
	$(CC) -c auxMethods.c
	
bloomfilter.o: bloomfilter.c
	$(CC) -c bloomfilter.c
	
hashTable.o: hashTable.c
	$(CC) -c hashTable.c	
	
murmur3.o: ./murmur3/murmur3.c
	$(CC) -c ./murmur3/murmur3.c

topK.o: topK.c
	$(CC) -c topK.c
	
compress.o: compress.c
	$(CC) -c compress.c	
	
batch_handler.o: batch_handler.c
	$(CC) -c batch_handler.c

JobScheduler.o: JobScheduler.c
	$(CC) -c JobScheduler.c		

clean:
	rm -f $(OBJECTS)

