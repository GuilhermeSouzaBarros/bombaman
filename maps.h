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

int colBarrier(Map *map, Rectangle target);
int checkCollisionEspecialX(Game* game, Player* player);
int checkCollisionEspecialY(Game* game, Player* player);

void mapSetup(Game* game, int num_map);
void draw_map(Map *map);
void drawEspecials(Game* game);

#endif