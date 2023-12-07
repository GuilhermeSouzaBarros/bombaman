#include "structs.h"
#include "raylib.h"

#ifndef PICKUP
#define PICKUP

void drawPickup(Texture2D* sprite, Pickup* pickups, int n);
void initPickup(Pickup* p, int x, int y);
void colPlayerPickups(Game* game, Player* player);

#endif