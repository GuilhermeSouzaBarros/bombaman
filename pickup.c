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

void colPlayerPickups(Game* game) {
    for (int i = 0; i < 2; i++) {
        Player* player = &game->players[i];
        for (int j = 0; j < game->total_pickups; j++) {
            if (CheckCollisionRecs(player->pos, game->pickups[j].pos)) {
                switch (game->pickups[j].type) {
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
                game->pickups[j] = game->pickups[game->total_pickups - 1]; 
                game->pickups[game->total_pickups-1].pos = (Rectangle){-40, -40, 0, 0}; 
                game->total_pickups -= 1;
                PlaySound(game->sounds[2]);
                return;
            }
        }
    }
}

void drawPickup(Game* game) {
    Texture2D* sprite = &game->pickups_sprite;
    Pickup* pickups = game->pickups;
    int n = game->total_pickups;
    for (int i = 0; i < n; i++) {
        if (pickups[i].visible) {
            int floating;
            Rectangle target = pickups[i].pos;
            switch (pickups[i].type) {
                case 0:
                    floating = (int)(10 * GetTime()) % 10;
                    if (floating > 4) {
                        floating = 10 - floating;
                    }
                    DrawTexturePro(*sprite, (Rectangle){0, 0, 320, 320}, target,
                    (Vector2){0, floating}, 0, WHITE);
                    break;
                case 1:
                    floating = (int)(3 + 10 * GetTime()) % 10;
                    if (floating > 4) {
                        floating = 10 - floating;
                    }
                    target.height = 22;
                    DrawTexturePro(*sprite, (Rectangle){634, 0, 320, 180}, target,
                    (Vector2){0, floating - 12}, 0, WHITE);
                    break;
                    break;
                case 2:
                    floating = (int)(6 + 10 * GetTime()) % 10;
                    if (floating > 4) {
                        floating = 10 - floating;
                    }
                    target.height = 48;
                    DrawTexturePro(*sprite, (Rectangle){420, 80, 120, 160}, target,
                    (Vector2){0, 8 + floating}, 0, WHITE);
                    break;
            }
        }
    }
}
