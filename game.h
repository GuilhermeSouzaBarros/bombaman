#include "raylib.h"

#include "structs.h"
#include "player.h"
#include "pickup.h"

#ifndef GAME
#define GAME

Game* initGame(int map, char* p1_nome, char* p2_nome, Font* font);
void freeGame(Game* game);
void gameLoop(Game *g);

#endif