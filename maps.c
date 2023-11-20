#include "maps.h"
#include <stdio.h>

int barrier_collision(Map *map, Rectangle target){
    for(int i = 0; i < map->num_barriers_line; i++){
        for (int j = 0; j < map->num_barriers_coln; j++) {
            if(CheckCollisionRecs(target, map->barriers[i][j])){
                return 1;
            }
        }
    }
    return 0;
}

void map0_setup(Map *map0){
    map0->num_barriers_line = 15;
    map0->num_barriers_coln = 15;
    for (int i = 0; i < map0->num_barriers_line; i++) {
        for (int j = 0; j < map0->num_barriers_coln; j++) {
            if ((i == 0 || j == 0 || i == 14 || j == 14) ||
                (i % 2 == 0 && j % 2 == 0)) {
                map0->barriers[i][j] = (Rectangle) {40*i, 40*j, 40, 40};
            } else {
                map0->barriers[i][j] = (Rectangle) {-40, -40, 0, 0};
            }
        }
    }
    map0->color = BLUE;
    map0->prev_map = -1;
    map0->next_map = 1;
}

void draw_map(Map *map){
    for(int i = 0; i < map->num_barriers_line; i++){
        for (int j = 0; j < map->num_barriers_coln; j++) {
            DrawRectangleRec(map->barriers[i][j], GRAY);
        }
    }
}