CC=g++
CFLAGS=-I.

access: access.o
	$(CC) -o access access.o 
