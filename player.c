#include "player.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void initPlayer(Game* game, Player* player, char* nome, Color color, Rectangle pos) {
    player->pos = pos;
    player->color = color;
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
        }
    }
    player->vivo = 1;
    player->facing = 0;
    player->is_moving = 0;
    player->sprite = LoadTexture("sprites/sans_spritesheet.png");
    player->sprite_bomb = LoadTexture("sprites/bob-omb sprite.png");
    player->sprite_explosion = LoadTexture("sprites/bomba_sprite.png");
}

int updateMovement(Game* game, Player* player, float* cord, int speed) {
    while (speed) {
        int col=0;
        *cord += speed;
        if (colBarrier(&game->map, player->pos) ||
            colBombasRec(player->pos, game->players[0].bombs, game->players[0].num_bombs) ||
            colBombasRec(player->pos, game->players[1].bombs, game->players[1].num_bombs)) {
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
    vel_x = updateMovement(game, &game->players[0], &game->players[0].pos.x, vel_x);
    vel_y = updateMovement(game, &game->players[0], &game->players[0].pos.y, vel_y);
    if (!(vel_x || vel_y)) {
        game->players[0].is_moving = 0;
        if (game->map.map_num == 0) {
            vel_x = checkCollisionEspecialX(game, &game->players[0]);
            vel_y = checkCollisionEspecialY(game, &game->players[0]);
            updateMovement(game, &game->players[0], &game->players[0].pos.x, vel_x);
            updateMovement(game, &game->players[0], &game->players[0].pos.y, vel_y);
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
    vel_x = updateMovement(game, &game->players[1], &game->players[1].pos.x, vel_x);
    vel_y = updateMovement(game, &game->players[1], &game->players[1].pos.y, vel_y);
    if (!(vel_x || vel_y) && game->map.map_num == 0) {
        vel_x = checkCollisionEspecialX(game, &game->players[1]);
        vel_y = checkCollisionEspecialY(game, &game->players[1]);
        updateMovement(game, &game->players[1], &game->players[1].pos.x, vel_x);
        updateMovement(game, &game->players[1], &game->players[1].pos.y, vel_y);
    }
}

void colPucciPlayer(Game* game, Player* player, int p) {
    if (game->time < 60) return; 
    if (CheckCollisionRecs(player->pos, *game->map.especial)) {
        game->map.pucci_pickup_steal_info[0] = player->speed - 2;
        game->map.pucci_pickup_steal_info[1] = player->num_bombs - 1;
        game->map.pucci_pickup_steal_info[2] = player->bomb_distance - 1;
        game->map.pucci_pickup_steal_info[3] = 1;
        game->map.pucci_pickup_steal_info[4] = p;
        player->speed = 2;
        player->num_bombs = 1;
        player->bomb_distance = 1;
        game->map.pucci_steal_time = GetTime();
    }
}

void drawPlayerSprite(Game* game, Player* player) {
    DrawTexturePro(player->sprite,
                   (Rectangle){ 28 * (!(!player->is_moving)) * (((int)(((2 * player->speed + 6) * game->time)) % 3 + 1)),
                                51 * player->facing + 1, 25, 30},
                   (Rectangle){player->pos.x + 2, player->pos.y, 30, 36},
                   (Vector2){0, 0},
                   0, WHITE);
}
