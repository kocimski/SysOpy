CC = gcc
FLAGS = -std=c99 -Wall
LIB_NAME = library
LIB_PATH = ../zad1
REPORT_FILE = raport2.txt

static: main.c
	$(CC) $(FLAGS)  main.c -o main -I$(LIB_PATH) $(LIB_PATH)/$(LIB_NAME).a

shared:
	$(CC) $(FLAGS) main.c -o main -L$(LIB_PATH) -I$(LIB_PATH) library.so

dynamic:
	$(CC) $(FLAGS) -I$(LIB_PATH) main.c -ldl -D DYNAMIC -o main

test:
	make static
	rm -f $(REPORT_FILE)
	LD_LIBRARY_PATH=$(LIB_PATH) ./main $(shell cat args.txt)
	mv raport.txt $(REPORT_FILE)

