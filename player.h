#include "structs.h"
#include "raylib.h"
#include "bombas.h"
#include "maps.h"

#ifndef PLAYER
#define PLAYER

void initPlayer(Player* player, char* nome, Color color, Rectangle pos);
void colPlayerPickups(Player* player, Pickup** pickups, int* n_pickups);
void updatePlayersPos(Game* game);

#endif