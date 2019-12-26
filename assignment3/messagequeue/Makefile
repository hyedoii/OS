all : processB processA

CC = gcc

processB : processB.o
	gcc -o processB processB.o

processB.o : processB.c
	gcc -c -o  processB.o processB.c

processA : processA.o
	gcc -o processA processA.o

processA.o : processA.c
	gcc -c -o processA.o processA.c

clean:
	rm *.o processA processB
