bomba: main.o game.o player.o bombas.o maps.o menu.o pickup.o structs.o
	gcc game.o main.o player.o bombas.o maps.o menu.o pickup.o structs.o -o bomba -lraylib -lm

main.o: main.c structs.o
	gcc -g -c main.c

game.o: game.c game.h structs.o maps.o player.o pickup.o
	gcc -g -c game.c

player.o: player.c player.h structs.o bombas.o maps.o
	gcc -g -c player.c

bombas.o: bombas.c bombas.h structs.o maps.o pickup.o
	gcc -g -c bombas.c

menu.o: menu.c menu.h structs.o maps.o
	gcc -g -c menu.c

pickup.o: pickup.c pickup.h structs.o
	gcc -g -c pickup.c

maps.o: maps.c maps.h structs.o
	gcc -g -c maps.c

structs.o: structs.c structs.h
	gcc -g -c structs.c
