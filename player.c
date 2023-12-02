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
    player->bomb_distance = 1;
    for (int i = 0; i < 5; i++) {
        player->bombs[i].isActive = 0;
        player->bombs[i].pos = (Rectangle){0, 0, 0, 0};
        for (int j = 0; j < 4; j++) {
            player->bombs[i].explosions[j] = (Rectangle){0, 0, 0, 0};
        }
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

void colPlayerPickups(Player* player, Pickup** pickups, int* n_pickups) {
    for (int i = 0; i < *n_pickups; i++) {
        if (CheckCollisionRecs(player->pos, pickups[i]->pos)) {
            switch (pickups[i]->type) {
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
            free(pickups[i]);
            pickups[i] = pickups[*n_pickups - 1]; 
            *n_pickups -= 1;
        }
    }
}

void updateMovement(Game* game, Player* player, float* cord, int speed) {
    while (speed) {
        int col=0;
        *cord += speed;
        if (colBarrier(&game->map, player->pos) ||
            colBombaPlayer(player->pos, game->players[0].bombs, game->players[0].num_bombs) ||
            colBombaPlayer(player->pos, game->players[1].bombs, game->players[1].num_bombs)) {
            *cord -= speed;
        } else {
            break;
        }
        if (speed > 0) speed--;
        else speed++;
    }
}

void updatePlayersPos(Game* game){
    int vel_x = 0, vel_y = 0;
    if (IsKeyDown(KEY_W)) vel_y -= game->players[0].speed;
    if (IsKeyDown(KEY_A)) vel_x -= game->players[0].speed;
    if (IsKeyDown(KEY_S)) vel_y += game->players[0].speed;
    if (IsKeyDown(KEY_D)) vel_x += game->players[0].speed;
    updateMovement(game, &game->players[0], &game->players[0].pos.x, vel_x);
    updateMovement(game, &game->players[0], &game->players[0].pos.y, vel_y);

    vel_x = 0, vel_y = 0;
    if (IsKeyDown(KEY_I)) vel_y -= game->players[1].speed;
    if (IsKeyDown(KEY_J)) vel_x -= game->players[1].speed;
    if (IsKeyDown(KEY_K)) vel_y += game->players[1].speed;
    if (IsKeyDown(KEY_L)) vel_x += game->players[1].speed;
    updateMovement(game, &game->players[1], &game->players[1].pos.x, vel_x);
    updateMovement(game, &game->players[1], &game->players[1].pos.y, vel_y);
}
