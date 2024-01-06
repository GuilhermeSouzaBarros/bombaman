#include "bombas.h"
#include "raylib.h"

#include <math.h>
#include <stdio.h>

void drawBombTexture(Texture2D* sprite, Bomb* bomb) {
    double bomb_time = GetTime() - bomb->time;
    int bomb_frame = (int)(10 * (bomb_time)) % 8, sprite_x = 975, sprite_y = 1275;
    bomb_time = (int)bomb_time;

    DrawTexturePro(*sprite, (Rectangle){bomb_frame % 2 * sprite_x, bomb_time * sprite_y, sprite_x, sprite_y},
    (Rectangle) {bomb->pos.x + 4, bomb->pos.y - 8, 36, 48},
    (Vector2){STD_SIZE_DIF, STD_SIZE_DIF}, 0, WHITE);
}

void drawExplosionTexture(Texture2D* sprite, Bomb* bomb) {
    int sprite_size = 16, sprite_dif = 17;
    int tempo_dif = (int)(4 * (GetTime() - bomb->time)) % 4;
    switch (tempo_dif)
    {
    case 1:
        tempo_dif = 82;
        break;
    case 2:
        tempo_dif = 166;
        break;
    case 3:
        tempo_dif = 251;
        break;
    }
    int tiles = bomb->explosions[0].height / 40 - 1;
    for (int i = 1; i <= tiles; i++) {
            Rectangle target = bomb->explosions[0];
            target.y += (i - 1) * STD_SIZE;
            target.height = 40;
        if (i == 1) {
            DrawTexturePro(*sprite,
            (Rectangle){sprite_dif * 2 + tempo_dif, 0 * sprite_dif, sprite_size, sprite_size},
            target, (Vector2){0, 0}, 0, WHITE);
        } else {
            DrawTexturePro(*sprite,
            (Rectangle){sprite_dif * 2 + tempo_dif, 1 * sprite_dif, sprite_size, sprite_size},
            target, (Vector2){0, 0}, 0, WHITE);
        }
    }
    tiles = bomb->explosions[1].width / 40 - 1;
    for (int i = 1; i <= tiles; i++) {
            Rectangle target = bomb->explosions[1];
            target.x += (i - 1) * STD_SIZE;
            target.width = 40;
        if (i == 1) {
            DrawTexturePro(*sprite,
            (Rectangle){sprite_dif * 0 + tempo_dif, 2 * sprite_dif, sprite_size, sprite_size},
            target, (Vector2){0, 0}, 0, WHITE);
        } else {
            DrawTexturePro(*sprite,
            (Rectangle){sprite_dif * 1 + tempo_dif, 2 * sprite_dif, sprite_size, sprite_size},
            target, (Vector2){0, 0}, 0, WHITE);
        }
    }
    tiles = bomb->explosions[2].height / 40 - 1;
    for (int i = 1; i <= tiles; i++) {
            Rectangle target = bomb->explosions[2];
            target.y += i * STD_SIZE;
            target.height = 40;
        if (i == tiles) {
            DrawTexturePro(*sprite,
            (Rectangle){sprite_dif * 2 + tempo_dif, 4 * sprite_dif, sprite_size, sprite_size},
            target, (Vector2){0, 0}, 0, WHITE);
        } else {
            DrawTexturePro(*sprite,
            (Rectangle){sprite_dif * 2 + tempo_dif, 3 * sprite_dif, sprite_size, sprite_size},
            target, (Vector2){0, 0}, 0, WHITE);
        }
    }
    tiles = bomb->explosions[3].width / 40 - 1;
    for (int i = 1; i <= tiles; i++) {
            Rectangle target = bomb->explosions[3];
            target.x += i * STD_SIZE;
            target.width = 40;
        if (i == tiles) {
            DrawTexturePro(*sprite,
            (Rectangle){sprite_dif * 4 + tempo_dif, 2 * sprite_dif, sprite_size, sprite_size},
            target, (Vector2){0, 0}, 0, WHITE);
        } else {
            DrawTexturePro(*sprite,
            (Rectangle){sprite_dif * 3 + tempo_dif, 2 * sprite_dif, sprite_size, sprite_size},
            target, (Vector2){0, 0}, 0, WHITE);
        }
    }
    Rectangle target = bomb->pos;
    target.x -= STD_SIZE_DIF;
    target.y -= STD_SIZE_DIF;
    target.width = STD_SIZE;
    target.height = STD_SIZE;
    DrawTexturePro(*sprite,
        (Rectangle){sprite_dif * 2 + tempo_dif, 2 * sprite_dif, sprite_size, sprite_size},
        target, (Vector2){0, 0}, 0, WHITE);
}

void drawBombs(Game* game) {
    Player* players = game->players;
    for (int i = 0; i < 2; i++) {
        for(int j = 0; j < players[i].num_bombs; j++){
            if(players[i].bombs[j].isActive){
                if (!players[i].bombs[j].hasExploded) {
                    drawBombTexture(&players[i].sprite_bomb, &players[i].bombs[j]);
                } else {
                    drawExplosionTexture(&players[i].sprite_explosion, &players[i].bombs[j]);
                }
            }
        }
    }
}

int tileHasBomb(Game* game, int x, int y) {
    x += STD_SIZE_ENT/2;
    y += STD_SIZE_ENT/2;
    x /= STD_SIZE;
    y /= STD_SIZE;
    Bomb bomb_p1, bomb_p2;
    for (int i = 0; i < 5; i ++) {
        bomb_p1 = game->players[0].bombs[i];
        bomb_p2 = game->players[1].bombs[i];
        if (bomb_p1.isActive || bomb_p2.isActive) {
            if ((((int)bomb_p1.pos.x)/STD_SIZE == x && ((int)bomb_p1.pos.y)/STD_SIZE == y) || 
                (((int)bomb_p2.pos.x)/STD_SIZE == x && ((int)bomb_p2.pos.y)/STD_SIZE == y)) {
                return 1;
            }
        }
    }
    return 0;
}

void initBomb(Game* game, Player player, Bomb* bomb) {
    int x = player.pos.x + STD_SIZE_ENT/2, y = player.pos.y + STD_SIZE_ENT/2;
    x = x/STD_SIZE * STD_SIZE + STD_SIZE_DIF;
    y = y/STD_SIZE * STD_SIZE + STD_SIZE_DIF;
    bomb->pos = (Rectangle) {x, y, STD_SIZE_ENT, STD_SIZE_ENT};
    bomb->distance = player.bomb_distance;
    bomb->isActive = 1;
    bomb->isActiveFirstFrame = 1;
    bomb->hasExploded = 0;
    bomb->time = GetTime();
    for (int i = 0; i < 2; i++) {
        bomb->hasColision[i] = 0;
    }
    bomb->fastExplode = 0;
}

void colBombas(Game* game) {
    Bomb* bomb;
    for (int i = 0; i < game->players[0].num_bombs; i++) {
        if (game->players[0].bombs[i].isActive && !game->players[0].bombs[i].hasExploded) {
            bomb = &game->players[0].bombs[i];
            if (colPerBombaExplosion(bomb, game)) {
                bomb->fastExplode = 1;
                bomb->time = GetTime() - 2;
                break;
            }
        }
    }
    for (int i = 0; i < game->players[1].num_bombs; i++) {
        if (game->players[1].bombs[i].isActive && !game->players[1].bombs[i].hasExploded) {
            bomb = &game->players[1].bombs[i];
            if (colPerBombaExplosion(bomb, game)) {
                bomb->fastExplode = 1;
                bomb->time = GetTime() - 2;
                break;
            }
        }
    }
}

int colBombasRecPerPlayer(Rectangle target, int player, Bomb bombs[], int n_bombs) {
    for (int i = 0; i < n_bombs; i++) {
        if (bombs[i].hasColision[player]) {
            if (CheckCollisionRecs(bombs[i].pos, target)) {
                return 1;
            }
        }
    }
    return 0;
}

int colDestroyable(Game* game, Rectangle explosion) {
    for(int i = 0; i < game->map.num_barriers_line; i++){
        for (int j = 0; j < game->map.num_barriers_coln; j++) {
            if (game->map.barriers.types[i][j] == 2 || game->map.barriers.types[i][j] == 3) {
                if(CheckCollisionRecs(explosion, game->map.barriers.barriers[i][j])){
                    game->map.barriers.types[i][j] = 0;
                    int random = 20 + rand() % 2 * 160;
                    game->map.barriers.sprite_pos[i][j] = (Rectangle){random, 100, 20, 20};
                    if (i == 1) {
                        game->map.barriers.sprite_pos[i][j] = (Rectangle){20, 80, 20, 20};
                    }
                    if (i == 13) {
                        game->map.barriers.sprite_pos[i][j] = (Rectangle){20, 120, 20, 20};
                    }
                    if (game->map.map_num == 1) {
                        game->map.barriers.sprite_pos[i][j] = (Rectangle){0, 0, 0, 0};
                    }
                    for (int k = 0; k < game->total_pickups; k++) {
                        if (game->pickups[k].tile_x == i && game->pickups[k].tile_y == j) {
                            game->pickups[k].visible = 1;
                            break;
                        }
                    }
                    return 1;
                }
            }
        }
    }
    return 0;
}

void placeBomb (Game* game, Player* player) {
    int hero_tile_x = ((int)player->pos.x + STD_SIZE_ENT / 2) / STD_SIZE;
    int hero_tile_y = ((int)player->pos.y + STD_SIZE_ENT / 2) / STD_SIZE;
    if (!tileHasBomb(game, (int)player->pos.x, (int)player->pos.y)) {
        for(int i = 0; i < player->num_bombs; i++){
            if(!player->bombs[i].isActive){
                initBomb(game, *player, &player->bombs[i]);
                PlaySound(player->bombs[i].sounds[1]);
                return;
            }
        }
    }
}

void explodeBombs(Game* game, Player* player){
    for(int i = 0; i < player->num_bombs; i++){
        if(player->bombs[i].isActive){
            Bomb* bomb = &player->bombs[i];
            if (((fabs(bomb->time - GetTime()) > 2 && fabs(bomb->time - GetTime()) < 3) && !game->end_game) ||
                (bomb->fastExplode || bomb->hasExploded)){
                if (bomb->isActiveFirstFrame) {
                    PlaySound(player->bombs[i].sounds[0]);
                    for (int j = 0; j < 4; j++) {
                        bomb->explosions[j] = (Rectangle){bomb->pos.x - STD_SIZE_DIF, bomb->pos.y - STD_SIZE_DIF, STD_SIZE, STD_SIZE};
                        bomb->stop_explosion[j] = 0;
                    }
                    bomb->hasExploded = 1;
                    bomb->isActiveFirstFrame = 0;
                } else {
                    if ((bomb->explosions[0].height/STD_SIZE - 1 < bomb->distance) && !bomb->stop_explosion[0]) {
                        Rectangle next_step = bomb->explosions[0];
                        next_step.y -= STD_SIZE;
                        next_step.height += STD_SIZE;
                        if (colDestroyable(game, next_step)) bomb->stop_explosion[0] = 1;
                        if (!colBarrier(&game->map, next_step)) {
                            bomb->explosions[0] = next_step;
                        }
                    }
                    if ((bomb->explosions[1].width/STD_SIZE - 1 < bomb->distance) && !bomb->stop_explosion[1]) {
                        Rectangle next_step = bomb->explosions[1];
                        next_step.x -= STD_SIZE;
                        next_step.width += STD_SIZE;
                        if (colDestroyable(game, next_step)) bomb->stop_explosion[1] = 1;
                        if (!colBarrier(&game->map, next_step)) {
                            bomb->explosions[1] = next_step;
                        }
                    }
                    if ((bomb->explosions[2].height/STD_SIZE - 1 < bomb->distance) && !bomb->stop_explosion[2]) {
                        Rectangle next_step = bomb->explosions[2];
                        next_step.height += STD_SIZE;
                        if (colDestroyable(game, next_step)) bomb->stop_explosion[2] = 1;
                        if (!colBarrier(&game->map, next_step)) {
                            bomb->explosions[2] = next_step;
                        }
                    }
                    if ((bomb->explosions[3].width/STD_SIZE - 1 < bomb->distance) && !bomb->stop_explosion[3]) {
                        Rectangle next_step = bomb->explosions[3];
                        next_step.width += STD_SIZE;
                        if (colDestroyable(game, next_step)) bomb->stop_explosion[3] = 1;
                        if (!colBarrier(&game->map, next_step)) {
                            bomb->explosions[3] = next_step;
                        }
                    }
                }
            }
            if ((fabs(bomb->time - GetTime()) > 3) && (!game->end_game || game->time > 120)){
                bomb->pos = (Rectangle) {0, 0, 0, 0};
                for (int j = 0; j < 4; j++) {
                    bomb->explosions[j] = (Rectangle) {0, 0, 0, 0};
                    bomb->stop_explosion[j] = 0;
                }
                bomb->isActive = 0;
                for (int i = 0; i < 2; i++) {
                    bomb->hasColision[i] = 0;
                }
            }
        }
    }
}

void setBombCol(Game* game, Bomb* bomb) {
    if (!(CheckCollisionRecs(bomb->pos, game->players[0].pos))) {
        bomb->hasColision[0] = 1;
    }
    if (!(CheckCollisionRecs(bomb->pos, game->players[1].pos))) {
        bomb->hasColision[1] = 1;
    }
}

void updateBombs(Game* game) {
    if(IsKeyPressed(KEY_SPACE)) {
        placeBomb(game, &game->players[0]);
    }
    if(IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER)) {
        placeBomb(game, &game->players[1]);
    }
    colBombas(game);
    for (int i = 0; i < game->players[0].num_bombs; i++) {
        if (game->players[0].bombs[i].isActive) {
            setBombCol(game, &game->players[0].bombs[i]);
        }
    }
    for (int i = 0; i < game->players[1].num_bombs; i++) {
        if (game->players[1].bombs[i].isActive) {
            setBombCol(game, &game->players[1].bombs[i]);
        }
    }
    explodeBombs(game, &game->players[0]);
    explodeBombs(game, &game->players[1]);
}
