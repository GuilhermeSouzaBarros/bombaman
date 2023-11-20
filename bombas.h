#include "maps.h"

#ifndef BOMBAS
#define BOMBAS

typedef struct Bomb{
    Rectangle pos;
    Rectangle explosion_right;
    Rectangle explosion_left;
    Rectangle explosion_up;
    Rectangle explosion_down;
    int growth_ratio;
    int isActive;
    int isActiveFirstFrame;
    int hasExploded;
    int distance;
    double time;
    int hasColision;
    int fastExplode;
}Bomb;

void draw_bomb(Bomb bombs[], int n);
void update_bomb(Map *actual_map, Rectangle player, Bomb bombs[], int n);

#endif
