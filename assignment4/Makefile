all : thread

CC = gcc

thread : thread.o
	gcc -o thread thread.o -pthread

thread.o : thread.c
	gcc -c -o thread.o thread.c -pthread

clean :
	rm *.o thread
