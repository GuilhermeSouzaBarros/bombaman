#include "player.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void initPlayer(Game* game, Player* player, char* nome, Rectangle pos, Color color) {
    player->pos = pos;
    player->nome = nome;
    player->speed = 4;
    player->num_bombs = 1;
    player->bomb_distance = 1;
    for (int i = 0; i < 5; i++) {
        player->bombs[i].isActive = 0;
        player->bombs[i].pos = (Rectangle){0, 0, 0, 0};
        for (int j = 0; j < 4; j++) {
            player->bombs[i].explosions[j] = (Rectangle){0, 0, 0, 0};
            player->bombs[i].stop_explosion[j] = 0;
        }
        for (int j = 0; j < 2; j++) {
            player->bombs[i].sounds[j] = LoadSoundAlias(game->sounds[j]);
            player->bombs[i].hasColision[j] = 0;
        }
    }
    player->vivo = 1;
    player->facing = 0;
    player->is_moving = 0;
    if (player == &game->players[0]) {
        player->sprite.self = LoadTexture("sprites/sans_spritesheet.png");
        player->sprite.x_dif = 28;
        player->sprite.y_dif = 51;
        player->sprite.x_size = 25;
        player->sprite.y_size = 32;
        player->sprite.offset = (Vector2){0, 2};
        player->sprite.death_frame = (Rectangle){56, 204, 25, 32};
    } else {
        player->sprite.self = LoadTexture("sprites/isaac_spritesheet.png");
        player->sprite.x_dif = 30;
        player->sprite.y_dif = 36;
        player->sprite.x_size = 30;
        player->sprite.y_size = 36;
        player->sprite.offset = (Vector2){0, 0};
        player->sprite.death_frame = (Rectangle){60, 145, 37, 35};
    }
    player->sprite_bomb = LoadTexture("sprites/bob-omb sprite.png");
    player->sprite_explosion = LoadTexture("sprites/bomba_sprite.png");
    player->color = color;
}

void freePlayer (Player* player) {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 2; j++) {
            UnloadSoundAlias(player->bombs[i].sounds[j]);
        }
    }
    UnloadTexture(player->sprite.self);
    UnloadTexture(player->sprite_bomb);
    UnloadTexture(player->sprite_explosion);
}

int updateMovement(Game* game, int player, float* cord, int speed) {
    while (speed) {
        int col=0;
        *cord += speed;
        if (colBarrier(&game->map, game->players[player].pos) ||
            colBombasRecPerPlayer(game->players[player].pos, player, game->players[0].bombs, game->players[0].num_bombs) ||
            colBombasRecPerPlayer(game->players[player].pos, player, game->players[1].bombs, game->players[1].num_bombs)) {
            *cord -= speed;
        } else {
            return speed;
        }
        if (speed > 0) speed--;
        else speed++;
    }
    return 0;
}

void updatePlayersPos(Game* game){
    int vel_x = 0, vel_y = 0;
    if (IsKeyDown(KEY_W)) vel_y -= game->players[0].speed;
    if (IsKeyDown(KEY_A)) vel_x -= game->players[0].speed;
    if (IsKeyDown(KEY_S)) vel_y += game->players[0].speed;
    if (IsKeyDown(KEY_D)) vel_x += game->players[0].speed;
    vel_x = updateMovement(game, 0, &game->players[0].pos.x, vel_x);
    vel_y = updateMovement(game, 0, &game->players[0].pos.y, vel_y);
    if (!(vel_x || vel_y)) {
        game->players[0].is_moving = 0;
        if (game->map.map_num == 0) {
            vel_x = checkCollisionEspecialX(game, &game->players[0]);
            vel_y = checkCollisionEspecialY(game, &game->players[0]);
            updateMovement(game, 0, &game->players[0].pos.x, vel_x);
            updateMovement(game, 0, &game->players[0].pos.y, vel_y);
        }
    } else {
        game->players[0].is_moving = 1;
        if (vel_y > 0) {
            game->players[0].facing = 0;
        } else if (vel_y < 0) {
            game->players[0].facing = 3;
        }
        if (vel_x > 0) {
            game->players[0].facing = 2;
        } else if (vel_x < 0) {
            game->players[0].facing = 1;
        }
    }

    vel_x = 0, vel_y = 0;
    if (IsKeyDown(KEY_I) || IsKeyDown(KEY_UP))    vel_y -= game->players[1].speed;
    if (IsKeyDown(KEY_J) || IsKeyDown(KEY_LEFT))  vel_x -= game->players[1].speed;
    if (IsKeyDown(KEY_K) || IsKeyDown(KEY_DOWN))  vel_y += game->players[1].speed;
    if (IsKeyDown(KEY_L) || IsKeyDown(KEY_RIGHT)) vel_x += game->players[1].speed;
    vel_x = updateMovement(game, 1, &game->players[1].pos.x, vel_x);
    vel_y = updateMovement(game, 1, &game->players[1].pos.y, vel_y);
    if (!(vel_x || vel_y)) {
        game->players[1].is_moving = 0;
        if (game->map.map_num == 0) {
            vel_x = checkCollisionEspecialX(game, &game->players[1]);
            vel_y = checkCollisionEspecialY(game, &game->players[1]);
            updateMovement(game, 1, &game->players[1].pos.x, vel_x);
            updateMovement(game, 1, &game->players[1].pos.y, vel_y);
        }
    } else {
        game->players[1].is_moving = 1;
        if (vel_y > 0) {
            game->players[1].facing = 0;
        } else if (vel_y < 0) {
            game->players[1].facing = 3;
        }
        if (vel_x > 0) {
            game->players[1].facing = 2;
        } else if (vel_x < 0) {
            game->players[1].facing = 1;
        }
    }
}

void colDeliriumPlayer(Game* game, Player* player, int p) {
    if (CheckCollisionRecs(player->pos, *game->map.especial)) {
        game->map.delirium_pickup_steal_info[0] = player->speed - 2;
        game->map.delirium_pickup_steal_info[1] = player->num_bombs - 1;
        game->map.delirium_pickup_steal_info[2] = player->bomb_distance - 1;
        game->map.delirium_pickup_steal_info[3] = 1;
        game->map.delirium_pickup_steal_info[4] = p;
        player->speed = 2;
        player->num_bombs = 1;
        player->bomb_distance = 1;
        game->map.delirium_steal_time = game->time;
    }
}

void drawPlayersSprite(Game* game) {
    for (int i = 0; i < 2; i++) {
        Player* player = &game->players[i];
        if (player->vivo) {
            DrawTexturePro(player->sprite.self,
                    (Rectangle){player->sprite.x_dif * player->is_moving * (((int)(((2 * player->speed) * game->time)) % 4)),
                                player->sprite.y_dif * player->facing, player->sprite.x_size, player->sprite.y_size},
                    (Rectangle){player->pos.x, player->pos.y, 30, 36 + player->sprite.offset.y},
                    player->sprite.offset,
                    0, WHITE);
        } else {
            int frame = (int)(2 * game->end_time) - (int)(2 * game->time);
            switch (frame) {
                case 5 ... 6:
                    DrawTexturePro(player->sprite.self,
                    (Rectangle){0, player->sprite.y_dif * 4, player->sprite.x_size, player->sprite.y_size},
                    (Rectangle){player->pos.x, player->pos.y, 30, 36},
                    player->sprite.offset,
                    0, WHITE);
                    break;
                case 4:
                    DrawTexturePro(player->sprite.self,
                        (Rectangle){player->sprite.x_dif, player->sprite.y_dif * 4, player->sprite.x_size, player->sprite.y_size},
                        (Rectangle){player->pos.x, player->pos.y, 30, 36},
                        player->sprite.offset,
                        0, WHITE);
                    break;
                case 0 ... 3:
                    DrawTexturePro(player->sprite.self,
                                player->sprite.death_frame,
                        (Rectangle){player->pos.x, player->pos.y, 36, 36},
                        player->sprite.offset,
                        0, WHITE);
                    break;
            }
        }
    }
}
