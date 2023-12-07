#include "pickup.h"

#include <stdio.h>

void initPickup(Pickup* p, int x, int y) {
    p->visible = 0;
    static int type = 0;
    int actual_type = type / 2;
    p->tile_x = x;
    p->tile_y = y; 
    p->pos = (Rectangle){y*STD_SIZE + STD_SIZE_DIF, x * STD_SIZE + STD_SIZE_DIF, STD_SIZE_ENT, STD_SIZE_ENT};
    if (actual_type == 0) {
        p->type = 0;
        p->color = BLUE;
    } else if (0 < actual_type && actual_type <= 2) {
        p->type = 1;
        p->color = YELLOW;
    } else {
        p->type = 2;
        p->color = GREEN;
    }
    type++;
    type %= 10;
}

void colPlayerPickups(Game* game, Player* player) {
    for (int i = 0; i < game->total_pickups; i++) {
        if (CheckCollisionRecs(player->pos, game->pickups[i].pos)) {
            switch (game->pickups[i].type) {
                case 0:
                    player->speed++;
                    break;
                case 1:
                    player->num_bombs++;
                    break;
                case 2:
                    player->bomb_distance++;
                    break;
            }
            game->pickups[i] = game->pickups[game->total_pickups - 1]; 
            game->pickups[game->total_pickups-1].pos = (Rectangle){-40, -40, 0, 0}; 
            game->total_pickups -= 1;
            return;
        }
    }
}

void drawPickup(Pickup* pickups, int n) {
    for (int i = 0; i < n; i++) {
        if (pickups[i].visible) {
            DrawRectangleRec(pickups[i].pos, pickups[i].color);
        }
    }
}
