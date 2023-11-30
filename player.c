#include "player.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void initPlayer(Player* player, char* nome, Color color, Rectangle pos) {
    player->pos = pos;
    player->color = color;
    strcpy(player->nome, nome);
    player->speed = 4;
    player->num_bombs = 1;
    for (int i = 0; i < 5; i++) {
        player->bombs[i].isActive = 0;
        player->bombs[i].distance = 80;
        player->bombs[i].explosion_up    = (Rectangle){-40, -40, 0, 0};
        player->bombs[i].explosion_left  = (Rectangle){-40, -40, 0, 0};
        player->bombs[i].explosion_down  = (Rectangle){-40, -40, 0, 0};
        player->bombs[i].explosion_right = (Rectangle){-40, -40, 0, 0};
    }
    player->vivo = 1;
}

int colBombaPlayer(Rectangle player, Bomb bombs[], int n_bombs) {
    for (int i = 0; i < n_bombs; i++) {
        if (bombs[i].hasColision) {
            if (CheckCollisionRecs(bombs[i].pos, player)) {
                return 1;
            }
        }
    }
    return 0;
}

void colExplosionPlayer(Bomb bombs[], int n, Player *player) {
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

void colPlayerPickups(Player* player, Pickup** pickups, int* n_pickups) {
    for (int i = 0; i < *n_pickups; i++) {
        if (CheckCollisionRecs(player->pos, pickups[i]->pos)) {
            switch (pickups[i]->type) {
                case 0:
                    player->speed++;
                    break;
                case 1:
                    player->num_bombs++;
                    break;
                case 2:
                    for (int j = 0; j < 5; j++) {
                        player->bombs[i].distance += 40;
                    }
                    break;
            }
            free(pickups[i]);
            if (i != *n_pickups-1) {
                pickups[i] = pickups[*n_pickups-1]; 
            }
            *n_pickups--;
        }
    }
}

void updateMovement(Rectangle* player, float* cord, int speed,
    Map* m, Bomb bombs_p1[], int p1, Bomb bombs_p2[], int p2) {
    while (speed) {
        int col=0;
        *cord += speed;
        if (barrier_collision(m, *player) || colBombaPlayer(*player, bombs_p1, p1) ||
            colBombaPlayer(*player, bombs_p2, p2)) {
            *cord -= speed;
        } else {
            break;
        }
        if (speed > 0) speed--;
        else speed++;
    }
}

void updatePlayersPos(Player *p1, Player *p2, Map *m){
    int vel1_x = 0, vel1_y = 0;
    if (IsKeyDown(KEY_W)) vel1_y -= p1->speed;
    if (IsKeyDown(KEY_A)) vel1_x -= p1->speed;
    if (IsKeyDown(KEY_S)) vel1_y += p1->speed;
    if (IsKeyDown(KEY_D)) vel1_x += p1->speed;
    updateMovement(&p1->pos, &p1->pos.x, vel1_x, m, p1->bombs, p1->num_bombs, p2->bombs, p2->num_bombs);
    updateMovement(&p1->pos, &p1->pos.y, vel1_y, m, p1->bombs, p1->num_bombs, p2->bombs, p2->num_bombs);

    int vel2_x = 0, vel2_y = 0;
    if (IsKeyDown(KEY_I)) vel2_y -= p2->speed;
    if (IsKeyDown(KEY_J)) vel2_x -= p2->speed;
    if (IsKeyDown(KEY_K)) vel2_y += p2->speed;
    if (IsKeyDown(KEY_L)) vel2_x += p2->speed;
    updateMovement(&p2->pos, &p2->pos.x, vel2_x, m, p2->bombs, p2->num_bombs, p1->bombs, p1->num_bombs);
    updateMovement(&p2->pos, &p2->pos.y, vel2_y, m, p2->bombs, p2->num_bombs, p1->bombs, p1->num_bombs);
}
