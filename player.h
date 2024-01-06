#include "structs.h"
#include "raylib.h"
#include "bombas.h"
#include "maps.h"

#ifndef PLAYER
#define PLAYER

void initPlayer        (Game* game, Player* player, char* nome, Rectangle pos, Color color);
void colPlayerPickups  (Game* game, Player* player);
void colDeliriumPlayer (Game* game, Player* player, int p);
void updatePlayersPos  (Game* game);
void drawPlayersSprite (Game* game);

#endif