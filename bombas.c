#include "bombas.h"
#include "raylib.h"

#include <math.h>
#include <stdio.h>

void drawBombTexture(Texture2D* sprite, Bomb* bomb) {
    double bomb_time = GetTime() - bomb->time;
    int bomb_frame = (int)(10 * (bomb_time)) % 8, sprite_x = 975, sprite_y = 1275;
    bomb_time = (int)bomb_time;

    DrawTexturePro(*sprite, (Rectangle){bomb_frame % 2 * sprite_x, bomb_time * sprite_y, sprite_x, sprite_y},
    (Rectangle) {bomb->pos.x, bomb->pos.y - 8, 36, 48},
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
    DrawTexturePro(*sprite, (Rectangle){sprite_dif * 2 + tempo_dif, 2 * sprite_dif, sprite_size, sprite_size},
        bomb->pos, (Vector2){0, 0}, 0, WHITE);
}

void draw_bomb(Player* player){
    for(int i = 0; i < player->num_bombs; i++){
        if(player->bombs[i].isActive == 1){
            if (player->bombs[i].isActiveFirstFrame && !player->bombs[i].hasExploded) {
                drawBombTexture(&player->sprite_bomb, &player->bombs[i]);
            } else {
                drawExplosionTexture(&player->sprite_explosion, &player->bombs[i]);
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
    bomb->hasColision = 0;
    bomb->fastExplode = 0;
}

void colBombas(Game* game) {
    Bomb* bomb;
    for (int i = 0; i < game->players[0].num_bombs; i++) {
        if (game->players[0].bombs[i].isActive && !game->players[0].bombs[i].hasExploded) {
            bomb = &game->players[0].bombs[i];
            if (colPerBombaExplosion(bomb, game)) {
                bomb->fastExplode = 1;
                break;
            }
        }
    }
    for (int i = 0; i < game->players[1].num_bombs; i++) {
        if (game->players[1].bombs[i].isActive && !game->players[1].bombs[i].hasExploded) {
            bomb = &game->players[1].bombs[i];
            if (colPerBombaExplosion(bomb, game)) {
                bomb->fastExplode = 1;
                break;
            }
        }
    }
}

int colBombasRec(Rectangle target, Bomb bombs[], int n_bombs) {
    for (int i = 0; i < n_bombs; i++) {
        if (bombs[i].hasColision) {
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
                return;
            }
        }
    }
}

void explodeBombs(Game* game, Player* player){
    for(int i = 0; i < player->num_bombs; i++){
        if(player->bombs[i].isActive){
            Bomb* bomb = &player->bombs[i];
            if ((fabs(bomb->time - GetTime()) > 2 && fabs(bomb->time - GetTime()) < 3) || bomb->fastExplode){
                if (bomb->isActiveFirstFrame) {
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
            if (fabs(bomb->time - GetTime()) > 3){
                bomb->pos = (Rectangle) {0, 0, 0, 0};
                for (int j = 0; j < 4; j++) {
                    bomb->explosions[j] = (Rectangle) {0, 0, 0, 0};
                    bomb->stop_explosion[j] = 0;
                }
                bomb->isActive = 0;
                bomb->hasColision = 0;
            }
        }
    }
}

void setBombCol(Game* game, Bomb* bomb) {
    if (!(CheckCollisionRecs(bomb->pos, game->players[0].pos) ||
          CheckCollisionRecs(bomb->pos, game->players[1].pos))) {
        bomb->hasColision = 1;
    }
}

void updateBombs(Game* game) {
    if(IsKeyPressed(KEY_SPACE)) {
        placeBomb(game, &game->players[0]);
    }
    if(IsKeyPressed(KEY_ENTER)) {
        placeBomb(game, &game->players[1]);
    }
    colBombas(game);
    for (int i = 0; i < game->players[0].num_bombs; i++) {
        if (game->players[0].bombs[i].isActive) {
            if (!game->players[0].bombs[i].hasColision) {
                setBombCol(game, &game->players[0].bombs[i]);
            }
        }
    }
    for (int i = 0; i < game->players[1].num_bombs; i++) {
        if (game->players[1].bombs[i].isActive) {
            if (!game->players[1].bombs[i].hasColision) {
                setBombCol(game, &game->players[1].bombs[i]);
            }
        }
    }
    explodeBombs(game, &game->players[0]);
    explodeBombs(game, &game->players[1]);
}
