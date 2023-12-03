#include "bombas.h"
#include "raylib.h"

#include <math.h>
#include <stdio.h>

void draw_bomb(Bomb bombs[], int n){
    for(int i = 0; i < n; i++){
        if(bombs[i].isActive == 1){
            for (int j = 0; j < 4; j++) {
                DrawRectangleRec(bombs[i].explosions[j], RED);
            }
            if (bombs[i].isActiveFirstFrame) {
                DrawRectangleRec(bombs[i].pos, BLACK);
            }
        }
    }
}

int tileHasBomb(Game* game, int x, int y) {
    x += 16;
    y += 16;
    x /= 40;
    y /= 40;
    Bomb bomb_p1, bomb_p2;
    for (int i = 0; i < 5; i ++) {
        bomb_p1 = game->players[0].bombs[i];
        bomb_p2 = game->players[1].bombs[i];
        if (bomb_p1.isActive || bomb_p2.isActive) {
            if ((((int)bomb_p1.pos.x)/40 == x && ((int)bomb_p1.pos.y)/40 == y) || 
                (((int)bomb_p2.pos.x)/40 == x && ((int)bomb_p2.pos.y)/40 == y)) {
                return 1;
            }
        }
    }
    return 0;
}

void initBomb(Game* game, Player player, Bomb* bomb) {
    int x = player.pos.x + 16, y = player.pos.y + 16;
    x = x/40 * 40 + 4;
    y = y/40 * 40 + 4;
    bomb->pos = (Rectangle) {x, y, STD_SIZE_ENT, STD_SIZE_ENT};
    bomb->distance = player.bomb_distance;
    bomb->isActive = 1;
    bomb->isActiveFirstFrame = 1;
    bomb->hasExploded = 0;
    bomb->time = GetTime();
    bomb->hasColision = 0;
    bomb->fastExplode = 0;
}

int colExplosion (Bomb bombs[], int n, Rectangle target) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < 4; j++) {
            if (CheckCollisionRecs(target, bombs[i].explosions[j]) &&
                    bombs[i].hasExploded) {
                return 1;
            }
        }
    }
    return 0;
}

int colPerBombaExplosion(Bomb* bomb, Game* game) {
    for (int i = 0; i < game->players[0].num_bombs; i++) {
        if (bomb == &game->players[0].bombs[i]) continue;
        if (game->players[0].bombs[i].isActive && game->players[0].bombs[i].hasExploded) {
            if (colExplosion(game->players[0].bombs, game->players[0].num_bombs, bomb->pos)) {
                return 1;
            }
        }
    }
    for (int i = 0; i < game->players[1].num_bombs; i++) {
        if (bomb == &game->players[1].bombs[i]) continue;
        if (game->players[1].bombs[i].isActive && game->players[1].bombs[i].hasExploded) {
            if (colExplosion(game->players[1].bombs, game->players[1].num_bombs, bomb->pos)) {
                return 1;
            }
        }
    }
    return 0;
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

void colDestroyable(Game* game, Rectangle explosion) {
    for(int i = 0; i < game->map.num_barriers_line; i++){
        for (int j = 0; j < game->map.num_barriers_coln; j++) {
            if (game->map.barriers.types[i][j] == 2 || game->map.barriers.types[i][j] == 3) {
                if(CheckCollisionRecs(explosion, game->map.barriers.barriers[i][j])){
                    game->map.barriers.types[i][j] = 0;
                }
            }
        }
    }
}

void placeBomb (Game* game, Player* player) {
    int hero_tile_x = ((int)player->pos.x + STD_SIZE_ENT / 2) / 40;
    int hero_tile_y = ((int)player->pos.y + STD_SIZE_ENT / 2) / 40;
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
            if ((fabs(bomb->time - GetTime()) > 3 && fabs(bomb->time - GetTime()) < 4) || bomb->fastExplode){
                if (bomb->isActiveFirstFrame) {
                    for (int j = 0; j < 4; j++) {
                        bomb->explosions[j] = (Rectangle){bomb->pos.x - STD_SIZE_DIF, bomb->pos.y - STD_SIZE_DIF, STD_SIZE, STD_SIZE};
                    }
                    bomb->hasExploded = 1;
                    bomb->isActiveFirstFrame = 0;
                } else {
                    if (bomb->explosions[0].height/STD_SIZE - 1 < bomb->distance) {
                        Rectangle next_step = bomb->explosions[0];
                        next_step.y -= STD_SIZE;
                        next_step.height += STD_SIZE;
                        colDestroyable(game, next_step);
                        if (!colBarrier(&game->map, next_step)) {
                            bomb->explosions[0] = next_step;
                        }
                    }
                    if (bomb->explosions[1].width/STD_SIZE - 1 < bomb->distance) {
                        Rectangle next_step = bomb->explosions[1];
                        next_step.x -= STD_SIZE;
                        next_step.width += STD_SIZE;
                        colDestroyable(game, next_step);
                        if (!colBarrier(&game->map, next_step)) {
                            bomb->explosions[1] = next_step;
                        }
                    }
                    if (bomb->explosions[2].height/STD_SIZE - 1 < bomb->distance) {
                        Rectangle next_step = bomb->explosions[2];
                        next_step.height += STD_SIZE;
                        colDestroyable(game, next_step);
                        if (!colBarrier(&game->map, next_step)) {
                            bomb->explosions[2] = next_step;
                        }
                    }
                    if (bomb->explosions[3].width/STD_SIZE - 1 < bomb->distance) {
                        Rectangle next_step = bomb->explosions[3];
                        next_step.width += STD_SIZE;
                        colDestroyable(game, next_step);
                        if (!colBarrier(&game->map, next_step)) {
                            bomb->explosions[3] = next_step;
                        }
                    }
                }
            }
            if (fabs(bomb->time - GetTime()) > 4){
                bomb->pos = (Rectangle) {0, 0, 0, 0};
                for (int j = 0; j < 4; j++) {
                    bomb->explosions[j] = (Rectangle) {0, 0, 0, 0};
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
