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
void updateBombs(Map *map,
    Rectangle player1, Bomb player1bombs[], int player1nb,
    Rectangle player2, Bomb player2bombs[], int player2nb);

#endif
