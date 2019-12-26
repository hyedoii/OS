all : main

CC = gcc

main : main.o
	gcc -o main main.o -pthread

main.o : main.c
	gcc -c -o main.o main.c -pthread

clean :
	rm *.o main
