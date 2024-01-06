#include "structs.h"
#include "raylib.h"
#include "stdlib.h"

#ifndef MAPS
#define MAPS

int colExplosion            (Bomb bombs[], int n, Rectangle target);
int colPerBombaExplosion    (Bomb* bomb, Game* game);
int colBarrier              (Map *map, Rectangle target);
int checkCollisionEspecialX (Game* game, Player* player);
int checkCollisionEspecialY (Game* game, Player* player);

void mapSetup       (Game* game, int num_map);
void drawMap        (Game* game);
void drawEspecials  (Game* game);
void updateDelirium (Game* game);

#endif