#include "structs.h"
#include "maps.h"
#include "pickup.h"

#ifndef BOMBAS
#define BOMBAS

void drawBombs(Game* game);
void updateBombs(Game* game);
int colExplosion (Bomb bombs[], int n, Rectangle target);
int colBombasRecPerPlayer(Rectangle target, int player, Bomb bombs[], int n_bombs);

#endif
