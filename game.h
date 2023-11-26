#include "raylib.h"
#include "player.h"

#ifndef GAME
#define GAME

typedef struct Game {
    Map map;
    Player player1;
    Player player2;
} Game;

Game* initGame(Map map, char* p1_nome, char* p2_nome);
void updateGame(Game *g);
void DrawGame(Game *g);
void gameLoop(Game *g);

#endif