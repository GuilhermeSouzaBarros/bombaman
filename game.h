#include "raylib.h"

#include "structs.h"
#include "player.h"
#include "pickup.h"

#ifndef GAME
#define GAME

Game* initGame(Map map, char* p1_nome, char* p2_nome);
void updateGame(Game *g);
void DrawGame(Game *g);
void gameLoop(Game *g);

#endif