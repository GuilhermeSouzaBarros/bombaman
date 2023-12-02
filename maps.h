#include "structs.h"
#include "raylib.h"
#include "stdlib.h"

#ifndef MAPS
#define MAPS

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define STD_SIZE 40
#define STD_SIZE_ENT 32
#define STD_SIZE_DIF 4
#define SCREEN_BORDER 4

int colBarrier(Map *map, Rectangle target);
Map mapSetup(int num_map);
void draw_map(Map *map);

#endif