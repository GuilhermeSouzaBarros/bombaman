#include "structs.h"
#include "raylib.h"

#ifndef PICKUP
#define PICKUP

void drawPickup(Game* game);
void initPickup(Pickup* p, int x, int y);
void colPlayerPickups(Game* game);

#endif