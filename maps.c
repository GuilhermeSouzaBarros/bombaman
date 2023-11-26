#include "maps.h"
#include <stdio.h>

int barrier_collision(Map *map, Rectangle target){
    for(int i = 0; i < map->num_barriers_line; i++){
        for (int j = 0; j < map->num_barriers_coln; j++) {
            if(CheckCollisionRecs(target, map->barriers.barriers[i][j]) &&
                map->barriers.types[i][j] == 1){
                return 1;
            }
        }
    }
    return 0;
}

Map map0_setup(){
    Map map;
    map.num_barriers_line = 15;
    map.num_barriers_coln = 15;
    for (int i = 0; i < map.num_barriers_line; i++) {
        for (int j = 0; j < map.num_barriers_coln; j++) {
            if ((i == 0 || j == 0 || i == 14 || j == 14) ||
                (i % 2 == 0 && j % 2 == 0)) {
                map.barriers.barriers[i][j] = (Rectangle) {40*i, 40*j, 40, 40};
                map.barriers.types[i][j] = 1;
            } else {
                map.barriers.barriers[i][j] = (Rectangle) {-40, -40, 0, 0};
                map.barriers.types[i][j] = 0;
            }
        }
    }
    map.color = BLUE;
    return map;
}

Map mapSetup(int num_map) {
    switch (num_map) {
        case 0:
            return map0_setup();
    }
}

void draw_map(Map *map){
    for(int i = 0; i < map->num_barriers_line; i++){
        for (int j = 0; j < map->num_barriers_coln; j++) {
            DrawRectangleRec(map->barriers.barriers[i][j], GRAY);
        }
    }
}