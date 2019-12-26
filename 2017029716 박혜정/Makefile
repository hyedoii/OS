all : child1 child2 main

CC = gcc

child1 : child1.o
	gcc -o child1 child1.o

child1.o : child1.c
	gcc -c -o child1.o child1.c

child2 : child2.o
	gcc -o child2 child2.o

child2.o : child2.c
	gcc -c -o child2.o child2.c

main : main.o
	gcc -o main main.o

main.o : main.c
	gcc -c -o main.o main.c

clean:
	rm *.o main child1 child2

