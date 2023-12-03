#include "raylib.h"

#include "structs.h"
#include "player.h"
#include "pickup.h"

#ifndef GAME
#define GAME

Game* initGame(Map map, char* p1_nome, char* p2_nome);
void freeGame(Game* game);
void gameLoop(Game *g, Placar* placar);

#endif