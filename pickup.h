#include "raylib.h"
#include "maps.h"

#ifndef PICKUP
#define PICKUP

typedef struct Pickup {
    Rectangle pos;
    Color color;
    int type;
} Pickup;

void drawPickup(Pickup** pickups, int n);
Pickup* initPickup(int x, int y);

#endif