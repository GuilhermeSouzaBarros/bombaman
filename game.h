#include "raylib.h"
#include "player.h"
#include "pickup.h"

#ifndef GAME
#define GAME

typedef struct Game {
    Map map;
    Player player1;
    Player player2;
    Pickup* pickups[10];
    int n_pickups;
    double time;
} Game;

Game* initGame(Map map, char* p1_nome, char* p2_nome);
int timesUp(double time);
void updateGame(Game *g);
void DrawGame(Game *g);
void gameLoop(Game *g);

#endif