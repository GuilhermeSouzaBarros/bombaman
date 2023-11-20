#include "raylib.h"
#include "player.h"

#ifndef GAME
#define GAME

typedef struct Game { 
    Map maps[10];
    int num_maps;
    int curr_map;
    Player player;
    int screenWidth;
    int screenHeight;
    int gameover;
} Game;

void InitGame(Game *g);
void UpdateGame(Game *g);
void DrawGame(Game *g);
void UpdateDrawFrame(Game *g);

#endif