#include "structs.h"
#include "raylib.h"

#ifndef PICKUP
#define PICKUP

void drawPickup(Pickup* pickups, int n);
void initPickup(Pickup* p, int x, int y);
void colPlayerPickups(Game* game, Player* player);

#endif