CC = gcc
FILES = main.c list.c auxMethods.c
OBJECTS = main.o list.o auxMethods.o
OUT = main
HEADERS = list.h auxMethods.h

all: $(OBJECTS) $(HEADERS)
	$(CC) -o $(OUT) $(FILES)
	make clean
	
main.o: main.c
	$(CC) -c main.c
	
list.o: list.c
	$(CC) -c list.c 

auxMethods.o: auxMethods.c
	$(CC) -c auxMethods.c


clean:
	rm -f $(OBJECTS)
