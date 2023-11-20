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

void update_bomb(Map *actual_map, Rectangle player, Bomb bombs[], int n){
    if(IsKeyPressed(KEY_SPACE)){
        int hero_tile_x = ((int)player.x + STD_SIZE_ENT_X / 2);
        hero_tile_x -= hero_tile_x % 40 - 4;
        int hero_tile_y = ((int)player.y + STD_SIZE_ENT_Y / 2);
        hero_tile_y -= hero_tile_y % 40 - 4;
        for(int i = 0; i < n; i++){
            if(bombs[i].isActive == 0){
                bombs[i].isActive = 1;
                bombs[i].isActiveFirstFrame = 1;
                bombs[i].growth_ratio = 7;
                bombs[i].pos = (Rectangle) {hero_tile_x, hero_tile_y, STD_SIZE_ENT_X, STD_SIZE_ENT_Y};
                bombs[i].explosion_right = (Rectangle) {hero_tile_x, hero_tile_y, STD_SIZE_ENT_X, STD_SIZE_ENT_Y};
                bombs[i].explosion_left = (Rectangle) {hero_tile_x, hero_tile_y, STD_SIZE_ENT_X, STD_SIZE_ENT_Y};
                bombs[i].explosion_down = (Rectangle) {hero_tile_x, hero_tile_y, STD_SIZE_ENT_X, STD_SIZE_ENT_Y};
                bombs[i].explosion_up = (Rectangle) {hero_tile_x, hero_tile_y, STD_SIZE_ENT_X, STD_SIZE_ENT_Y};
                bombs[i].time = GetTime();
                bombs[i].hasColision = 0;
                bombs[i].fastExplode = 0;
                break;
            }
        }
    }
    for(int i = 0; i < n; i++){
        if(bombs[i].isActive){
            if (!bombs[i].hasColision && !CheckCollisionRecs(bombs[i].pos, player)) {
                bombs[i].hasColision = 1;
                actual_map->barriers[((int)bombs[i].pos.x)/40][((int)bombs[i].pos.y)/40] = bombs[i].pos;
            }
            for (int j = 0; j < n; j++) {
                if (i != j) {
                    if (CheckCollisionRecs(bombs[i].pos, bombs[j].explosion_down)) bombs[i].fastExplode = 1;
                    if (CheckCollisionRecs(bombs[i].pos, bombs[j].explosion_up)) bombs[i].fastExplode = 1;
                    if (CheckCollisionRecs(bombs[i].pos, bombs[j].explosion_left)) bombs[i].fastExplode = 1;
                    if (CheckCollisionRecs(bombs[i].pos, bombs[j].explosion_right)) bombs[i].fastExplode = 1;
                }
            }
            if((fabs(bombs[i].time - GetTime()) > 3 && fabs(bombs[i].time - GetTime()) < 5) || bombs[i].fastExplode){
                if (bombs[i].isActiveFirstFrame) {
                    actual_map->barriers[((int)bombs[i].pos.x)/40][((int)bombs[i].pos.y)/40] = (Rectangle) {-40, -40, 0, 0};
                    bombs[i].explosion_right = (Rectangle){bombs[i].pos.x - STD_SIZE_DIF_X, bombs[i].pos.y - STD_SIZE_DIF_Y, STD_SIZE_X, STD_SIZE_Y};
                    bombs[i].explosion_left = (Rectangle){bombs[i].pos.x - STD_SIZE_DIF_X, bombs[i].pos.y - STD_SIZE_DIF_Y, STD_SIZE_X, STD_SIZE_Y};
                    bombs[i].explosion_up = (Rectangle){bombs[i].pos.x - STD_SIZE_DIF_X, bombs[i].pos.y - STD_SIZE_DIF_Y, STD_SIZE_X, STD_SIZE_Y};
                    bombs[i].explosion_down = (Rectangle){bombs[i].pos.x - STD_SIZE_DIF_X, bombs[i].pos.y - STD_SIZE_DIF_Y, STD_SIZE_X, STD_SIZE_Y};
                    bombs[i].isActiveFirstFrame = 0;
                }
                Rectangle verify_bomb;
                Rectangle rectangle_bomb;
                if (bombs[i].explosion_right.width < bombs[i].distance) {
                    rectangle_bomb = bombs[i].explosion_right;
                    Rectangle verify_bomb = (Rectangle){ rectangle_bomb.x, rectangle_bomb.y, rectangle_bomb.width + bombs[i].growth_ratio,  rectangle_bomb.height};
                    if(!barrier_collision(actual_map, verify_bomb)) {
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
                    if(!barrier_collision(actual_map, verify_bomb)){
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
                    if(!barrier_collision(actual_map, verify_bomb)){
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
                    if(!barrier_collision(actual_map, verify_bomb)) {
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
            if(fabs(bombs[i].time - GetTime()) > 5){
                bombs[i].isActive = 0;
            }
        }
    }
}