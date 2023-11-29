#include "raylib.h"
#include "stdlib.h"

#ifndef MAPS
#define MAPS


#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define STD_SIZE_X 40
#define STD_SIZE_Y 40
#define STD_SIZE_ENT_X 32
#define STD_SIZE_ENT_Y 32
#define STD_SIZE_DIF_X 4
#define STD_SIZE_DIF_Y 4
#define SCREEN_BORDER 4

typedef struct Barriers {
    Rectangle barriers[15][15];
    int types[15][15]; // 0: Nothing; 1: Wall; 2: Destructible
} Barriers;

typedef struct Map {
    Barriers barriers;
    int num_barriers_line;
    int num_barriers_coln;
    Color color;
} Map;

int barrier_collision(Map *map, Rectangle target);
Map mapSetup(int num_map);
void draw_map(Map *map);

#endif