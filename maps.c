#include "maps.h"
#include "pickup.h"

#include <stdio.h>

int colBarrier(Map *map, Rectangle target){
    for(int i = 0; i < map->num_barriers_line; i++){
        for (int j = 0; j < map->num_barriers_coln; j++) {
            if (map->barriers.types[i][j]) {
                if(CheckCollisionRecs(target, map->barriers.barriers[i][j])){
                    return 1;
                }
            }
        }
    }
    return 0;
}

void map0_setup(Game* game){
    Map* map = &game->map;
    map->num_barriers_line = 15;
    map->num_barriers_coln = 15;
    for (int i = 0; i < map->num_barriers_line; i++) {
        for (int j = 0; j < map->num_barriers_coln; j++) {
            map->barriers.barriers[i][j] = (Rectangle) {40*i, 40*j, 40, 40};
            map->barriers.types[i][j] = 0;
        }
    }
    int destructs = 0;
    for (int i = 0; i < map->num_barriers_line; i++) {
        for (int j = 0; i + j < map->num_barriers_coln; j++) {
            map->barriers.barriers[i][j] = (Rectangle) {40*i, 40*j, 40, 40};
            if ((i == 0 || j == 0) || (i % 2 == 0 && j % 2 == 0)) {
                map->barriers.types[i][j] = 1;
                map->barriers.types[14-i][14-j] = 1;
            } else if (i + j > 3) {
                if ((rand() % 5) < 3) {
                    map->barriers.types[i][j] = 2;
                    map->barriers.types[14-i][14-j] = 2;
                    destructs++;
                    if (i != j) {
                        destructs++;
                    }
                } 
            }
        }
    }
    int atual = 0;
    for (int i = 0; i < map->num_barriers_line; i++) {
        for (int j = 0; i + j < map->num_barriers_coln; j++) {
            if (map->barriers.types[i][j] != 2) continue;
            if ((atual % (destructs / 10) == destructs/20)) {
                if (game->total_pickups < 10) {
                    initPickup(&game->pickups[game->total_pickups], i, j);
                    map->barriers.types[i][j] = 3;
                    game->total_pickups += 1;
                    if (i != j || (i == j && game->total_pickups < 9));
                    initPickup(&game->pickups[game->total_pickups], 14-i, 14-j);
                    map->barriers.types[14-i][14-j] = 3;
                    game->total_pickups += 1;
                    
                }
            }
            atual++;
        }
    }
    map->color = GRAY;
}

void mapSetup(Game* game, int num_map) {
    switch (num_map) {
        case 0:
            map0_setup(game);
    }
}

void draw_map(Map *map){
    for(int i = 0; i < map->num_barriers_line; i++){
        for (int j = 0; j < map->num_barriers_coln; j++) {
            switch (map->barriers.types[i][j]) {
                case 1:
                    DrawRectangleRec(map->barriers.barriers[i][j], map->color);
                    if (i != 0 && i != 14 && j != 0 && j != 14) {
                        DrawRectangleLinesEx(map->barriers.barriers[i][j], 1, BLACK);
                    }
                    break;
                case 2:
                    DrawRectangleRec(map->barriers.barriers[i][j], BROWN);
                    break;
                case 3:
                    DrawRectangleRec(map->barriers.barriers[i][j], DARKBROWN);
                    break;
            }
        }
    }
    DrawRectangleLinesEx((Rectangle){39, 39, 522, 522}, 1, BLACK);
}
