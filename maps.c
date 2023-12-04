#include "maps.h"
#include "pickup.h"

#include <stdio.h>
#include <math.h>

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

int collisionEspecial0X(Game* game, Player* player) {
    for (int i = 0; i < game->map.n_especiais; i++) {
        if (CheckCollisionRecs(player->pos, game->map.especial[i])) {
            Rectangle colisao = GetCollisionRec(player->pos, game->map.especial[i]);
            if (i % 2) {
                if (player->pos.x + 16 <= colisao.x) {
                    return -colisao.width;
                } else {
                    return colisao.width;
                }
            }
        }
    }
    return 0;
}

int collisionEspecial0Y(Game* game, Player* player) {
    for (int i = 0; i < game->map.n_especiais; i++) {
        if (CheckCollisionRecs(player->pos, game->map.especial[i])) {
            Rectangle colisao = GetCollisionRec(player->pos, game->map.especial[i]);
            if (!(i % 2)) {
                if (player->pos.y + 16 <= colisao.y) {
                    return -colisao.height;
                } else {
                    return colisao.height;
                }
            }
        }
    }
    return 0;
}

int checkCollisionEspecialX(Game* game, Player* player) {
    switch(game->map.map_num) {
        case 0:
            return collisionEspecial0X(game, player);
    }
}

int checkCollisionEspecialY(Game* game, Player* player) {
    switch(game->map.map_num) {
        case 0:
            return collisionEspecial0Y(game, player);
    }
}

void baseSetup(Game* game){
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
            if (i == 7 && j == 7) continue;
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

void map0Especial(Map* map) {
    map->map_num = 0;
    Rectangle especiais[4];
    int n_rec = 2;
    int tile_x = rand() % 6 + 2;
    int tile_y = rand() % 6 + 2;
    
    especiais[0] = (Rectangle){0, STD_SIZE * tile_x + 16, 15 * STD_SIZE, 8};
    especiais[1] = (Rectangle){STD_SIZE * tile_y + 16, 0, 8, 15 * STD_SIZE};

    if (tile_x != 7) {
        especiais[2] = (Rectangle){0, STD_SIZE * (14 - tile_x) + 16, 15 * STD_SIZE, 8};
        n_rec++;
    }

    if (tile_y != 7) {
        especiais[3] = (Rectangle){STD_SIZE * (14 - tile_y) + 16, 0, 8, 15 * STD_SIZE};
        n_rec++;
    }

    map->n_especiais = n_rec;
    map->especial = (Rectangle*)malloc(sizeof(Rectangle) * n_rec);
    for (int i = 0; i < n_rec; i++) {
        map->especial[i] = especiais[i];
    }
}

int updatePucciMovement(Game* game, Rectangle* pucci, float* cord, int speed) {
    while (speed) {
        int col=0;
        *cord += speed;
        if (colBarrier(&game->map, *pucci) ||
            colExplosion(game->players[0].bombs, game->players[0].num_bombs, *pucci) ||
            colExplosion(game->players[1].bombs, game->players[1].num_bombs, *pucci)) {
            *cord -= speed;
        } else {
            return 1;
        }
        if (speed > 0) speed--;
        else speed++;
    }
    return 0;
}

void updatePucci(Game* game) {
    if (game->time < 60) return;
    int dist_p1 = sqrt(pow(game->players[0].pos.x - game->map.especial->x, 2) +
                       pow(game->players[0].pos.y - game->map.especial->y, 2));
    int dist_p2 = sqrt(pow(game->players[1].pos.x - game->map.especial->x, 2) +
                       pow(game->players[1].pos.y - game->map.especial->y, 2));
    Player player;
    if (dist_p1 > dist_p2) {
        player = game->players[1];
    } else {
        player = game->players[0];
    }
    if (player.pos.x > game->map.especial->x) {
        updatePucciMovement(game, game->map.especial, &(*game->map.especial).x, 2);
    } else if (player.pos.x < game->map.especial->x) {
        updatePucciMovement(game, game->map.especial, &(*game->map.especial).x, -2);
    }   
    if (player.pos.y > game->map.especial->y) {
        updatePucciMovement(game, game->map.especial, &(*game->map.especial).y, 2);
    } else if (player.pos.y < game->map.especial->y) {
        updatePucciMovement(game, game->map.especial, &(*game->map.especial).y, -2);
    }
}

void map1Especial(Map* map) {
    map->especial = (Rectangle*)malloc(sizeof(Rectangle));
    *(map->especial) = (Rectangle){7*STD_SIZE + STD_SIZE_DIF, 7*STD_SIZE + STD_SIZE_DIF, STD_SIZE_ENT, STD_SIZE_ENT};
    map->n_especiais = 1;
    map->map_num = 1;
    for (int i = 0; i < 4; i++) {
        map->pucci_pickup_steal_info[i] = 0;
    }
    map->pucci_steal_time = GetTime();
}

void mapSetup(Game* game, int num_map) {
    baseSetup(game);
    switch (num_map) {
        case 0:
            map0Especial(&game->map);
            break;
        case 1:
            map1Especial(&game->map);
            break;
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

void drawEspecials(Game* game) {
    switch (game->map.map_num) {
        case 0:
            for (int i = 0; i < game->map.n_especiais; i++) {
                DrawRectangleRec(game->map.especial[i], (Color){255, 128, 0, 128});
            }
            break;
        case 1:
            if (game->time > 60) {
                DrawRectangleRec(*game->map.especial, WHITE);
            }
            break;
    }
}
