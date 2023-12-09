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
            map->barriers.barriers[i][j] = (Rectangle) {40*j, 40*i, 40, 40};
            map->barriers.types[i][j] = 0;
        }
    }

    int destructs = 0;
    for (int i = 0; i < map->num_barriers_line; i++) {
        for (int j = 0; i + j < map->num_barriers_coln; j++) {
            if (i == 7 && j == 7) continue;
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

void map0SpriteSetup(Map* map) {
    map->sprite = (Texture2D*)malloc(sizeof(Texture2D));
    map->sprite[0] = LoadTexture("sprites/hotlandSprites.png");
    for (int i = 0; i < map->num_barriers_line; i++) {
        for (int j = 0; j < map->num_barriers_coln; j++) {
            switch(map->barriers.types[i][j]) {
                case 0:
                    if (i == 1) {
                        map->barriers.sprite_pos[i][j] = (Rectangle){20, 80, 20, 20};
                    } else if (i == 13) {
                        map->barriers.sprite_pos[i][j] = (Rectangle){20, 120, 20, 20};
                    } else {
                        int random = 20 + !(rand() % 3) * 160;
                        map->barriers.sprite_pos[i][j] = (Rectangle){random, 100, 20, 20};
                    }
                    break;
                case 1:
                    if (((i == 0 || i == 14) && j == 0)) {
                        map->barriers.sprite_pos[i][j] = (Rectangle){0, 140, 20, 20};
                    } else if ((i == 0 || i == 14) && j == 14) {
                        map->barriers.sprite_pos[i][j] = (Rectangle){40, 140, 20, 20};
                    } else if (i == 13 && j == 0) {
                        map->barriers.sprite_pos[i][j] = (Rectangle){260, 60, 20, 20};
                    } else if (i == 13 && j == 14) {
                        map->barriers.sprite_pos[i][j] = (Rectangle){200, 60, 20, 20};
                    } else if (i == 1 && j == 0) {
                        map->barriers.sprite_pos[i][j] = (Rectangle){260, 0, 20, 20};
                    } else if (i == 1 && j == 14) {
                        map->barriers.sprite_pos[i][j] = (Rectangle){200, 0, 20, 20};
                    } else if (i == 0) {
                        map->barriers.sprite_pos[i][j] = (Rectangle){80, 140, 20, 20};
                    } else if (i == 14) {
                        map->barriers.sprite_pos[i][j] = (Rectangle){80, 140, 20, 20};
                    } else if (j == 0) {
                        int random = !!(!(rand() % 3));
                        random = 20 * (random + 1);
                        map->barriers.sprite_pos[i][j] = (Rectangle){200, random, 20, 20};
                    } else if (j == 14) {
                        int random = !!(!(rand() % 3));
                        random = 20 * (random + 1);
                        map->barriers.sprite_pos[i][j] = (Rectangle){260, random, 20, 20};
                    } else {
                        map->barriers.sprite_pos[i][j] = (Rectangle){179, 241, 20, 20};
                    }
                    break;
                case 2:
                    int random = rand() % 2 * 20;
                    map->barriers.sprite_pos[i][j] = (Rectangle){160 + random, 100 - random, 20, 20};
                    break;
                case 3:
                    map->barriers.sprite_pos[i][j] = (Rectangle){240, 140, 120, 120};
                    break;
            }
        }
    }
}

void map0Setup(Map* map) {
    map0SpriteSetup(map);
    map->map_num = 0;
    Rectangle especiais[4];
    int n_rec = 2;
    int tile_x = rand() % 6 + 2;
    int tile_y = rand() % 6 + 2;
    
    especiais[0] = (Rectangle){0, STD_SIZE * tile_x + 16, 15 * STD_SIZE, 8};
    especiais[1] = (Rectangle){STD_SIZE * tile_y + 16, 0, 8, 15 * STD_SIZE};

    if (tile_x != 7) {
        especiais[n_rec] = (Rectangle){0, STD_SIZE * (14 - tile_x) + 16, 15 * STD_SIZE, 8};
        n_rec++;
    }

    if (tile_y != 7) {
        especiais[n_rec] = (Rectangle){STD_SIZE * (14 - tile_y) + 16, 0, 8, 15 * STD_SIZE};
        n_rec++;
    }

    map->n_especiais = n_rec;
    map->especial = (Rectangle*)malloc(sizeof(Rectangle) * n_rec);
    for (int i = 0; i < n_rec; i++) {
        map->especial[i] = especiais[i];
    }
}

int updateDeliriumMovement(Game* game, Rectangle* delirium, float* cord, int speed) {
    while (speed) {
        int col=0;
        *cord += speed;
        if (colBarrier(&game->map, *delirium) ||
            colExplosion(game->players[0].bombs, game->players[0].num_bombs, *delirium) ||
            colExplosion(game->players[1].bombs, game->players[1].num_bombs, *delirium)) {
            *cord -= speed;
        } else {
            return 1;
        }
        if (speed > 0) speed--;
        else speed++;
    }
    return 0;
}

void updateDelirium(Game* game) {
    if (game->time < 45) return;
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
        updateDeliriumMovement(game, game->map.especial, &(*game->map.especial).x, 2);
    } else if (player.pos.x < game->map.especial->x) {
        updateDeliriumMovement(game, game->map.especial, &(*game->map.especial).x, -2);
    }   
    if (player.pos.y > game->map.especial->y) {
        updateDeliriumMovement(game, game->map.especial, &(*game->map.especial).y, 2);
    } else if (player.pos.y < game->map.especial->y) {
        updateDeliriumMovement(game, game->map.especial, &(*game->map.especial).y, -2);
    }
}

void map1SpriteSetup(Map* map) {
    map->sprite = (Texture2D*)malloc(sizeof(Texture2D) * 3);
    map->sprite[0] = LoadTexture("images/Cathedral.png");
    map->sprite[1] = LoadTexture("sprites/cathedral.png");
    map->sprite[2] = LoadTexture("sprites/delirium.png");
    for (int i = 0; i < map->num_barriers_line; i++) {
        for (int j = 0; j < map->num_barriers_coln; j++) {
            switch(map->barriers.types[i][j]) {
                case 0:
                    map->barriers.sprite_pos[i][j] = (Rectangle){0, 0, 0, 0};
                    break;
                case 1:
                    map->barriers.sprite_pos[i][j] = (Rectangle){97, 0, 46, 83};
                    break;
                case 2:
                    int random = rand() % 2;
                    map->barriers.sprite_pos[i][j] = (Rectangle){random * 50, 0, 50 - 3 * random, 50 - 2*random};
                    break;
                case 3:
                    map->barriers.sprite_pos[i][j] = (Rectangle){0, 50, 36, 37};
                    break;
            }
        }
    }
}

void map1Setup(Map* map) {
    map1SpriteSetup(map);
    map->especial = (Rectangle*)malloc(sizeof(Rectangle));
    *(map->especial) = (Rectangle){7*STD_SIZE + STD_SIZE_DIF, 7*STD_SIZE + STD_SIZE_DIF, STD_SIZE_ENT, STD_SIZE_ENT};
    map->n_especiais = 1;
    map->map_num = 1;
    for (int i = 0; i < 4; i++) {
        map->delirium_pickup_steal_info[i] = 0;
    }
    map->delirium_steal_time = GetTime();
}

void mapSetup(Game* game, int num_map) {
    baseSetup(game);
    switch (num_map) {
        case 0:
            map0Setup(&game->map);
            break;
        case 1:
            map1Setup(&game->map);
            break;
    }
}

void drawMap0 (Map *map) {
    for(int i = 0; i < map->num_barriers_line; i++){
        for (int j = 0; j < map->num_barriers_coln; j++) {
            if (i == 1) {
                DrawTexturePro(map->sprite[0], (Rectangle){20, 80, 20, 20},
                map->barriers.barriers[i][j], (Vector2){0, 0}, 0, WHITE);
            }
            if (i == 13) {
                DrawTexturePro(map->sprite[0], (Rectangle){20, 120, 20, 20},
                map->barriers.barriers[i][j], (Vector2){0, 0}, 0, WHITE);
            }
            DrawTexturePro(map->sprite[0], map->barriers.sprite_pos[i][j],
            map->barriers.barriers[i][j], (Vector2){0, 0}, 0, WHITE);
        }
    }
}

void drawMap1(Map* map) {
    DrawTexturePro(map->sprite[0], (Rectangle){24, 41, 402, 261}, (Rectangle){0, 0, 600, 600},
                   (Vector2){0, 0}, 0, WHITE);
    for(int i = 1; i < map->num_barriers_line - 1; i++){
        for (int j = 1; j < map->num_barriers_coln - 1; j++) {
            DrawTexturePro(map->sprite[1], map->barriers.sprite_pos[i][j],
            map->barriers.barriers[i][j], (Vector2){0, 0}, 0, WHITE);
        }
    }

}

void draw_map(Map *map){
    if (map->map_num == 0) { 
        drawMap0(map);
        return;
    }
    drawMap1(map);
}

void drawEspecials(Game* game) {
    switch (game->map.map_num) {
        case 0:
            for (int i = 0; i < game->map.n_especiais; i++) {
                DrawRectangleRec(game->map.especial[i], (Color){255, 128, 0, 128});
                if (game->map.especial[i].height == 8) {
                    DrawTexturePro(game->map.sprite[0], (Rectangle){280, 260, 40, 40},
                    (Rectangle){game->map.especial[i].x, game->map.especial[i].y - 16, 40, 40},
                    (Vector2){0, 0}, 0, WHITE);
                } else {
                    DrawTexturePro(game->map.sprite[0], (Rectangle){320, 260, 40, 40},
                    (Rectangle){game->map.especial[i].x - 16, game->map.especial[i].y, 40, 40},
                    (Vector2){0, 0}, 0, WHITE);
                }
            }
            break;
        case 1:
            if (game->time <= 35) break;
            if (game->map.stun_delirium) {
                DrawTexturePro(game->map.sprite[2], (Rectangle){64 * 6, 64 * 4, 64, 64},
                    (Rectangle){game->map.especial->x, game->map.especial->y, 64, 64},
                    (Vector2){12, 6}, 0, WHITE);
            } else if (game->map.delirium_pickup_steal_info[3]) {
                DrawTexturePro(game->map.sprite[2], (Rectangle){64 * 6, 64 * 5, 64, 64},
                    (Rectangle){game->map.especial->x, game->map.especial->y, 64, 64},
                    (Vector2){12, 6}, 0, WHITE);
            } else if (game->time > 45) {
                int floating = (int)(3 * GetTime()) % 10;
                if (floating > 4) {
                    floating = 10 - floating;
                }
                int time_x = (int)(game->time * 6) % 6;
                int time_y = ((int)game->time * 6) % 12 / 6;
                DrawTexturePro(game->map.sprite[2], (Rectangle){64 * time_x, 64 * (2 + time_y), 64, 64},
                    (Rectangle){game->map.especial->x, game->map.especial->y, 64, 64},
                    (Vector2){12, 18 + floating}, 0, WHITE);

            } else if (game->time > 42) {
                int time = (int)game->time - 42;
                if (time != 2) {
                    DrawTexturePro(game->map.sprite[2], (Rectangle){64 * time, 64, 64, 64},
                    (Rectangle){STD_SIZE * 7, 7 * STD_SIZE, 64, 64}, (Vector2){12, 18}, 0, WHITE);

                } else {
                    DrawTexturePro(game->map.sprite[2], (Rectangle){64 * 7, 128, 64, 64},
                    (Rectangle){STD_SIZE * 7, 7 * STD_SIZE, 64, 64}, (Vector2){12, 18}, 0, WHITE);

                }
            } else if (game->time > 35) {
                int time = (game->time - 35) * STD_SIZE;
                DrawTexturePro(game->map.sprite[2], (Rectangle){64 * ((int)(game->time * 10) % 2), 0, 64, 64},
                    (Rectangle){STD_SIZE * 7, time + STD_SIZE_DIF, 64, 64}, (Vector2){12, 18}, 0, WHITE);
            }
            break;
    }
}
