#include "raylib.h"
#include "bombas.h"
#include "maps.h"

#ifndef PLAYER
#define PLAYER

typedef struct Player {
    Rectangle pos;
    Color color;
    int speed;
    int draw_bomb;
    int num_bombs;
    Bomb bombs[5];
} Player;

void update_player_pos(Player *h, Map *m);

#endif