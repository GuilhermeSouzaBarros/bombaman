#include "structs.h"
#include "maps.h"
#include "pickup.h"

#ifndef BOMBAS
#define BOMBAS

void draw_bomb(Bomb bombs[], int n);
void updateBombs(Game* game);
int colExplosion (Bomb bombs[], int n, Rectangle target);

#endif
