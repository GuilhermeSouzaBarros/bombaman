#include "raylib.h"
#include "bombas.h"
#include "maps.h"

#ifndef PLAYER
#define PLAYER

typedef struct Player {
    Rectangle pos;
    Color color;
    int speed;
    int num_bombs;
    Bomb bombs[5];
    int vivo;
} Player;

void initPlayer(Player* player, Color color, Rectangle pos);
void colBombaPlayer(Bomb bombs[], int n, Player *player);
void updatePlayersPos(Player* p1, Player* p2, Map *m);

#endif