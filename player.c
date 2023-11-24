#include "player.h"
#include <stdio.h>

void initPlayer(Player* player, Color color, Rectangle pos) {
    player->pos = pos;
    player->color = color;
    player->speed = 4;
    player->num_bombs = 5;
    for (int i = 0; i < 5; i++) {
        player->bombs[i].isActive = 0;
        player->bombs[i].distance = 80;
    }
    player->vivo = 1;
}

void colBombaPlayer(Bomb bombs[], int n, Player *player) {
    for (int i = 0; i < n; i++) {
        if (CheckCollisionRecs(player->pos, bombs[i].explosion_down) &&
                bombs[i].hasExploded) {
            player->vivo = 0;
            break;
        }
        if (CheckCollisionRecs(player->pos, bombs[i].explosion_up) &&
                bombs[i].hasExploded) {
            player->vivo = 0;
            break;
        }
        if (CheckCollisionRecs(player->pos, bombs[i].explosion_left) &&
                bombs[i].hasExploded) {
            player->vivo = 0;
            break;
        }
        if (CheckCollisionRecs(player->pos, bombs[i].explosion_right) &&
                bombs[i].hasExploded) {
            player->vivo = 0;
            break;
        }
    }
}

void updateMovement(Player* h, Map* m, int vel_x, int vel_y) {
    do {
        int col=0;
        h->pos.x += vel_x;
        for (int i = 0; i < m->num_barriers_line; i++) {
            for (int j = 0; j < m->num_barriers_coln; j++) {
                if (CheckCollisionRecs(m->barriers.barriers[i][j], h->pos)) {
                    h->pos.x -= vel_x;
                    if (vel_x > 0) { 
                        vel_x--;
                    } else {
                        vel_x++;
                    }
                    col = 1;
                    break;
                }
            }
        }
        if (col) {
            col = 0;
            continue;
        }
        vel_x = 0;
    } while (vel_x);
    do {
        int col=0;
        h->pos.y += vel_y;
        for (int i = 0; i < m->num_barriers_line; i++) {
            for (int j = 0; j < m->num_barriers_coln; j++) {
                if (CheckCollisionRecs(m->barriers.barriers[i][j], h->pos)) {
                    h->pos.y -= vel_y;
                    if (vel_y > 0) vel_y--; else vel_y++;
                    col = 1;
                    break;
                }
            }
        }
        if (col) {
            col = 0;
            continue;
        }
        vel_y = 0;
    } while (vel_y);
}

void updatePlayersPos(Player *p1, Player *p2, Map *m){
    int vel1_x = 0, vel1_y = 0;
    if (IsKeyDown(KEY_W)) vel1_y -= p1->speed;
    if (IsKeyDown(KEY_A)) vel1_x -= p1->speed;
    if (IsKeyDown(KEY_S)) vel1_y += p1->speed;
    if (IsKeyDown(KEY_D)) vel1_x += p1->speed;
    updateMovement(p1, m, vel1_x, vel1_y);

    int vel2_x = 0, vel2_y = 0;
    if (IsKeyDown(KEY_I)) vel2_y -= p2->speed;
    if (IsKeyDown(KEY_J)) vel2_x -= p2->speed;
    if (IsKeyDown(KEY_K)) vel2_y += p2->speed;
    if (IsKeyDown(KEY_L)) vel2_x += p2->speed;
    updateMovement(p2, m, vel2_x, vel2_y);
}
