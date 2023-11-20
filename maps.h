#include "raylib.h"

#ifndef MAPS
#define MAPS

#define STD_SIZE_X 40
#define STD_SIZE_Y 40
#define STD_SIZE_ENT_X 32
#define STD_SIZE_ENT_Y 32
#define STD_SIZE_DIF_X 4
#define STD_SIZE_DIF_Y 4
#define SCREEN_BORDER 4

typedef struct Map {
    Rectangle barriers[15][15];
    int num_barriers_line;
    int num_barriers_coln;
    Color color;
    int next_map;
    int prev_map;
} Map;

int barrier_collision(Map *map, Rectangle target);
void map0_setup(Map *map0);
void draw_map(Map *map);

#endif