CC = gcc -g
FILES = main.c struct.c func.c auxMethods.c stack.c bloomfilter.c ./murmur3/murmur3.c
OBJECTS = main.o struct.o func.o auxMethods.o stack.o bloomfilter.o murmur3.o
OUT = ngrams
HEADERS = struct.h func.h auxMethods.h stack.h bloomfilter.h ./murmur3/murmur3.h


all: $(OBJECTS) $(HEADERS)
	$(CC) -o $(OUT) $(FILES)
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
	
murmur3.o: ./murmur3/murmur3.c
	$(CC) -c ./murmur3/murmur3.c

clean:
	rm -f $(OBJECTS)

