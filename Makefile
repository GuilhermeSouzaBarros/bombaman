bomba: main.o game.o player.o bombas.o maps.o menu.o pickup.o
	gcc game.o main.o player.o bombas.o maps.o menu.o pickup.o -o bomba -lraylib -lm 

main.o: main.c
	gcc -g -c main.c

maps.o: maps.c maps.h
	gcc -g -c maps.c

player.o: player.c player.h
	gcc -g -c player.c

bombas.o: bombas.c bombas.h
	gcc -g -c bombas.c

game.o: game.c game.h
	gcc -g -c game.c

menu.o: menu.c menu.h
	gcc -g -c menu.c

pickup.o: pickup.c pickup.h
	gcc -g -c pickup.c