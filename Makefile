main: main.o funcoes.o
	gcc main.o funcoes.o -o main -lraylib -lm
main.o: main.c funcoes.h
	gcc -g -c main.c -lraylib -lm -lfuncoes
funcoes.o: funcoes.h funcoes.c
	gcc -g -c funcoes.c -lraylib -lm