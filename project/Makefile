CC = gcc
FILES = main.c struct.c func.c auxMethods.c 
OBJECTS = main.o struct.o func.o auxMethods.o
OUT = ngrams
HEADERS = struct.h func.h auxMethods.h

all: $(OBJECTS) $(HEADERS)
	$(CC) -o $(OUT) $(FILES)
	make clean
	
main.o: main.c
	$(CC) -c main.c

node.o: struct.c
	$(CC) -c struct.c
		
list.o: func.c
	$(CC) -c func.c 

auxMethods.o: auxMethods.c
	$(CC) -c auxMethods.c
	

clean:
	rm -f $(OBJECTS)
