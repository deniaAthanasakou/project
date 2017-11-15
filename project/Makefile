CC = gcc -g
FILES = main.c struct.c func.c auxMethods.c stack.c test.c
OBJECTS = main.o struct.o func.o auxMethods.o stack.o test.o
OUT = ngrams
HEADERS = struct.h func.h auxMethods.h stack.h test.h
OBJECTSTEST = struct.o func.o auxMethods.o stack.o test.o unittesting.o
FILESTEST = struct.c func.c auxMethods.c stack.c test.c unittesting.c

all: function unittesting
	make clean

function: $(OBJECTS) $(HEADERS)
	$(CC) -o $(OUT) $(FILES)
	
test.o: test.c
	$(CC) -c test.c	
	
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

unittesting: $(OBJECTSTEST) $(HEADERS)
	$(CC) -o unittesting $(FILESTEST)

unittesting.o: unittesting.c 
	$(CC) -c unittesting.c
	

clean:
	rm -f $(OBJECTS)
	rm -f unittesting.o
