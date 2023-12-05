#include "structs.h"
#include "raylib.h"
#include "bombas.h"
#include "maps.h"

#ifndef PLAYER
#define PLAYER

void initPlayer(Player* player, char* nome, Color color, Rectangle pos);
void colPlayerPickups(Game* game, Player* player);
void colPucciPlayer(Game* game, Player* player, int p);
void updatePlayersPos(Game* game);
void drawPlayerSprite(Game* game, Player* player);

#endif