#include "bombas.h"
#include "raylib.h"
#include <math.h>
#include <stdio.h>

void draw_bomb(Bomb bombs[], int n){
    for(int i = 0; i < n; i++){
        if(bombs[i].isActive == 1){
            DrawRectangleRec(bombs[i].explosion_right, RED);
            DrawRectangleRec(bombs[i].explosion_left, RED);
            DrawRectangleRec(bombs[i].explosion_up, RED);
            DrawRectangleRec(bombs[i].explosion_down, RED);
            if (bombs[i].isActiveFirstFrame) {
                DrawRectangleRec(bombs[i].pos, BLACK);
            }
        }
    }
}

void initBomb(Bomb* bomb, int x, int y, Map* map) {
    map->barriers.types[x][y] = 2;
    x = x * 40 + 4;
    y = y * 40 + 4;
    bomb->pos = (Rectangle) {x, y, STD_SIZE_ENT_X, STD_SIZE_ENT_Y};
    bomb->explosion_right = (Rectangle) {x, y, STD_SIZE_ENT_X, STD_SIZE_ENT_Y};
    bomb->explosion_left = (Rectangle) {x, y, STD_SIZE_ENT_X, STD_SIZE_ENT_Y};
    bomb->explosion_down = (Rectangle) {x, y, STD_SIZE_ENT_X, STD_SIZE_ENT_Y};
    bomb->explosion_up = (Rectangle) {x, y, STD_SIZE_ENT_X, STD_SIZE_ENT_Y};
    bomb->isActive = 1;
    bomb->isActiveFirstFrame = 1;
    bomb->hasExploded = 0;
    bomb->growth_ratio = 10;
    bomb->time = GetTime();
    bomb->hasColision = 0;
    bomb->fastExplode = 0;
}

void colPlayerBombas(Bomb bombs1[], int n, Bomb bombs2[], int m) {
    for (int i = 0; i < n; i++) {
        if (bombs1[i].isActive) {
            for (int j = 0; j < n; j++) {
                if (bombs2[j].isActive && bombs2[j].hasExploded) {
                    if (CheckCollisionRecs(bombs1[i].pos, bombs2[j].explosion_down)) {
                        bombs1[i].fastExplode = 1;
                        continue;
                    }
                    if (CheckCollisionRecs(bombs1[i].pos, bombs2[j].explosion_up)) {
                        bombs1[i].fastExplode = 1;
                        continue;
                    }
                    if (CheckCollisionRecs(bombs1[i].pos, bombs2[j].explosion_left)) {
                        bombs1[i].fastExplode = 1;
                        continue;
                    }
                    if (CheckCollisionRecs(bombs1[i].pos, bombs2[j].explosion_right)) {
                        bombs1[i].fastExplode = 1;
                        continue;
                    }
                }
            }
        }
    }
}

void colBombas(Bomb bombsp1[], int n, Bomb bombsp2[], int m) {
    colPlayerBombas(bombsp1, n, bombsp1, n);
    colPlayerBombas(bombsp1, n, bombsp2, m);
    colPlayerBombas(bombsp2, n, bombsp1, n);
    colPlayerBombas(bombsp2, n, bombsp2, m);
}

void placeBomb (Rectangle player, Map* map, Bomb bombs[], int n_bombs) {
    int hero_tile_x = ((int)player.x + STD_SIZE_ENT_X / 2) / 40;
    int hero_tile_y = ((int)player.y + STD_SIZE_ENT_Y / 2) / 40;
    if (!map->barriers.types[hero_tile_x][hero_tile_y]) {
        for(int i = 0; i < n_bombs; i++){
            if(bombs[i].isActive == 0){
                initBomb(&bombs[i], hero_tile_x, hero_tile_y, map);
                return;
            }
        }
    }
}

void explodeBombs(Map *map, Rectangle player, Bomb bombs[], int n){
    for(int i = 0; i < n; i++){
        if(bombs[i].isActive){
            if ((fabs(bombs[i].time - GetTime()) > 3 && fabs(bombs[i].time - GetTime()) < 5) || bombs[i].fastExplode){
                if (bombs[i].isActiveFirstFrame) {
                    map->barriers.barriers[((int)bombs[i].pos.x)/40][((int)bombs[i].pos.y)/40] = (Rectangle) {-40, -40, 0, 0};
                    map->barriers.types[((int)bombs[i].pos.x)/40][((int)bombs[i].pos.y)/40] = 0;
                    bombs[i].explosion_right = (Rectangle){bombs[i].pos.x - STD_SIZE_DIF_X, bombs[i].pos.y - STD_SIZE_DIF_Y, STD_SIZE_X, STD_SIZE_Y};
                    bombs[i].explosion_left = (Rectangle){bombs[i].pos.x - STD_SIZE_DIF_X, bombs[i].pos.y - STD_SIZE_DIF_Y, STD_SIZE_X, STD_SIZE_Y};
                    bombs[i].explosion_up = (Rectangle){bombs[i].pos.x - STD_SIZE_DIF_X, bombs[i].pos.y - STD_SIZE_DIF_Y, STD_SIZE_X, STD_SIZE_Y};
                    bombs[i].explosion_down = (Rectangle){bombs[i].pos.x - STD_SIZE_DIF_X, bombs[i].pos.y - STD_SIZE_DIF_Y, STD_SIZE_X, STD_SIZE_Y};
                    bombs[i].isActiveFirstFrame = 0;
                    bombs[i].hasExploded = 1;
                }
                Rectangle verify_bomb;
                Rectangle rectangle_bomb;
                if (bombs[i].explosion_right.width < bombs[i].distance) {
                    rectangle_bomb = bombs[i].explosion_right;
                    Rectangle verify_bomb = (Rectangle){ rectangle_bomb.x, rectangle_bomb.y, rectangle_bomb.width + bombs[i].growth_ratio,  rectangle_bomb.height};
                    if(!barrier_collision(map, verify_bomb)) {
                        if (verify_bomb.width < bombs[i].distance) {
                            bombs[i].explosion_right.width += bombs[i].growth_ratio;
                        } else {
                            bombs[i].explosion_right.width = bombs[i].distance;
                        }
                    } else {
                        if ((int)bombs[i].explosion_right.width % 40) {
                            bombs[i].explosion_right.width += 40 - (int)bombs[i].explosion_right.width % 40;
                        }
                    }
                }
                if (bombs[i].explosion_left.width < bombs[i].distance) {
                    rectangle_bomb = bombs[i].explosion_left;
                    Rectangle verify_bomb = (Rectangle){ rectangle_bomb.x - bombs[i].growth_ratio, rectangle_bomb.y, rectangle_bomb.width + bombs[i].growth_ratio,  rectangle_bomb.height};
                    if(!barrier_collision(map, verify_bomb)){
                        if (verify_bomb.width < bombs[i].distance) {
                            bombs[i].explosion_left.x -= bombs[i].growth_ratio;
                            bombs[i].explosion_left.width += bombs[i].growth_ratio;
                        } else {
                            bombs[i].explosion_left.x -= bombs[i].distance - bombs[i].explosion_left.width;
                            bombs[i].explosion_left.width = bombs[i].distance;
                        }
                    } else {
                        bombs[i].explosion_left.x -= (int)bombs[i].explosion_left.x % 40;
                        bombs[i].explosion_left.width -= (int)bombs[i].explosion_left.width % 40;
                    }
                }
                if (bombs[i].explosion_up.height < bombs[i].distance) {
                    rectangle_bomb = bombs[i].explosion_up;
                    Rectangle verify_bomb = (Rectangle){ rectangle_bomb.x, rectangle_bomb.y - bombs[i].growth_ratio, rectangle_bomb.width,  rectangle_bomb.height + bombs[i].growth_ratio};
                    if(!barrier_collision(map, verify_bomb)){
                        if (verify_bomb.height < bombs[i].distance) {
                            bombs[i].explosion_up.y -= bombs[i].growth_ratio;
                            bombs[i].explosion_up.height += bombs[i].growth_ratio;
                        } else {
                            bombs[i].explosion_up.y -= bombs[i].distance - bombs[i].explosion_up.height;
                            bombs[i].explosion_up.height = bombs[i].distance;
                        }
                    } else {
                        bombs[i].explosion_up.y -= (int)bombs[i].explosion_up.y % 40;
                        bombs[i].explosion_up.height -= (int)bombs[i].explosion_up.height % 40 ;
                    }
                }
                if (bombs[i].explosion_down.height < bombs[i].distance) {
                    rectangle_bomb = bombs[i].explosion_down;
                    Rectangle verify_bomb = (Rectangle){ rectangle_bomb.x, rectangle_bomb.y, rectangle_bomb.width,  rectangle_bomb.height + bombs[i].growth_ratio};
                    if(!barrier_collision(map, verify_bomb)) {
                        if (verify_bomb.height < bombs[i].distance) {
                            bombs[i].explosion_down.height += bombs[i].growth_ratio;
                        } else {
                            bombs[i].explosion_down.height = bombs[i].distance;
                        }
                    } else {
                        if ((int)bombs[i].explosion_down.height % 40) {
                            bombs[i].explosion_down.height += 40 - (int)bombs[i].explosion_down.height % 40;
                        }
                    }
                }
            }
            if (fabs(bombs[i].time - GetTime()) > 5){
                bombs[i].explosion_right = (Rectangle) {-40, -40, 0, 0};
                bombs[i].explosion_left = (Rectangle) {-40, -40, 0, 0};
                bombs[i].explosion_down = (Rectangle) {-40, -40, 0, 0};
                bombs[i].explosion_up = (Rectangle) {-40, -40, 0, 0};
                bombs[i].isActive = 0;
            }
        }
    }
}

void setBombCol(Rectangle player1, Rectangle player2, Bomb* bomb, Map* map) {
    if (!bomb->hasColision && !CheckCollisionRecs(bomb->pos, player1) && !CheckCollisionRecs(bomb->pos, player2)) {
        bomb->hasColision = 1;
        map->barriers.barriers[((int)bomb->pos.x)/40][((int)bomb->pos.y)/40] = bomb->pos;
        map->barriers.types[((int)bomb->pos.x)/40][((int)bomb->pos.y)/40] = 2;
    }
}

void updateBombs(Map *map, Rectangle player1, Bomb player1bombs[], int player1nb,
    Rectangle player2, Bomb player2bombs[], int player2nb) {
    if(IsKeyPressed(KEY_SPACE)) {
        placeBomb(player1, map, player1bombs, player1nb);
    }
    if(IsKeyPressed(KEY_ENTER)) {
        placeBomb(player2, map, player2bombs, player2nb);
    }
    colBombas(player1bombs, player1nb, player2bombs, player2nb);
    for (int i = 0; i < player1nb; i++) {
        if (player1bombs[i].isActive) {
            setBombCol(player1, player2, &player1bombs[i], map);
        }
    }
    for (int i = 0; i < player2nb; i++) {
        if (player2bombs[i].isActive) {
            setBombCol(player1, player2, &player2bombs[i], map);
        }
    }
    explodeBombs(map, player1, player1bombs, player1nb);
    explodeBombs(map, player2, player2bombs, player2nb);
}
