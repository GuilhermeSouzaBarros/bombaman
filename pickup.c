#include "pickup.h"

Pickup* initPickup(int x, int y) {
    Pickup* p = (Pickup*)malloc(sizeof(Pickup));
    p->pos = (Rectangle){x + 4, y + 4, STD_SIZE_ENT, STD_SIZE_ENT};
    p->type = rand() % 3;
    switch (p->type) {
        case 0:
            p->color = BLUE;
            break;
        case 1:
            p->color = YELLOW;
            break;
        case 2:
            p->color = GREEN;
            break;
    }
    return p;
}

void colPlayerPickups(Game* game, Player* player) {
    for (int i = 0; i < game->n_pickups; i++) {
        if (CheckCollisionRecs(player->pos, game->pickups[i]->pos)) {
            switch (game->pickups[i]->type) {
                case 0:
                    if (player->speed < 6) {
                        player->speed++;
                    }
                    break;
                case 1:
                    if (player->num_bombs < 5) {
                        player->num_bombs++;
                    }
                    break;
                case 2:
                    if (player->bomb_distance < 5) {
                        player->bomb_distance++;
                    }
                    break;
            }
            free(game->pickups[i]);
            game->pickups[i] = game->pickups[game->n_pickups - 1]; 
            game->n_pickups -= 1;
        }
    }
}

void drawPickup(Pickup** pickups, int n) {
    for (int i = 0; i < n; i++) {
        DrawRectangleRec(pickups[i]->pos, pickups[i]->color);
    }
}
