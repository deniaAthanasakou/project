CC = gcc
FILES = main.c node.c list.c auxMethods.c 
OBJECTS = main.o node.o list.o auxMethods.o
OUT = main
HEADERS = node.h list.h auxMethods.h

all: $(OBJECTS) $(HEADERS)
	$(CC) -o $(OUT) $(FILES)
	make clean
	
main.o: main.c
	$(CC) -c main.c

node.o: node.c
	$(CC) -c node.c
		
list.o: list.c
	$(CC) -c list.c 

auxMethods.o: auxMethods.c
	$(CC) -c auxMethods.c
	

clean:
	rm -f $(OBJECTS)
