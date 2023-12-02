#include "structs.h"
#include "raylib.h"
#include "maps.h"

#ifndef PICKUP
#define PICKUP

void drawPickup(Pickup** pickups, int n);
Pickup* initPickup(int x, int y);

#endif