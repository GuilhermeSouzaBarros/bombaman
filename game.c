#include "game.h"
#include "bombas.h"
#include "maps.h"

#include "raylib.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"

Game* initGame(int map, char* p1_nome, char* p2_nome){
    Game* g = (Game*)malloc(sizeof(Game));
    initPlayer(&g->players[0], p1_nome, GOLD, (Rectangle){44, 44, STD_SIZE_ENT, STD_SIZE_ENT});
    initPlayer(&g->players[1], p2_nome, WHITE, (Rectangle){524, 524, STD_SIZE_ENT, STD_SIZE_ENT});
    g->total_pickups = 0;
    g->start_time = GetTime();
    g->time = g->start_time - GetTime();
    mapSetup(g, map);
    return g;
}

void freeGame(Game* game) {
    free(game->map.especial);
    free(game);
}

void updateGame(Game* game) {
    game->time = GetTime() - game->start_time;

    if (game->time < 120) {
        updatePlayersPos(game);
    } else {
        for (int i = 0; i < 5; i++) {
            game->players[0].bombs[i].fastExplode = 1;
            game->players[1].bombs[i].fastExplode = 1;
        }
        int cabou = 1;
        for (int i = 0; i < 5; i ++) {
            if (game->players[0].bombs[i].isActive || game->players[1].bombs[i].isActive) {
                cabou = 0;
            }
        }
        if (cabou) {
            game->players[0].vivo = 0;
            game->players[1].vivo = 0;
        }
    }

    switch (game->map.map_num) {
        case 0:
            break;
        case 1:
            if (GetTime() - game->map.pucci_steal_time > 20 &&
                game->map.pucci_pickup_steal_info[3] == 1) {
                int p_indice = game->map.pucci_pickup_steal_info[4];
                game->players[p_indice].speed += game->map.pucci_pickup_steal_info[0];
                game->players[p_indice].num_bombs += game->map.pucci_pickup_steal_info[1];
                game->players[p_indice].bomb_distance += game->map.pucci_pickup_steal_info[2];
                game->map.pucci_pickup_steal_info[3] = 0;
            } 
            if (!game->map.pucci_pickup_steal_info[3]) {
                updatePucci(game);
                colPucciPlayer(game, &game->players[0], 0);
            }
            if (!game->map.pucci_pickup_steal_info[3]) colPucciPlayer(game, &game->players[1], 1);
            break;
    }

    colPlayerPickups(game, &game->players[0]);
    colPlayerPickups(game, &game->players[1]);
    updateBombs(game);
    if (colExplosion(game->players[0].bombs, game->players[0].num_bombs, game->players[0].pos) ||
        colExplosion(game->players[1].bombs, game->players[1].num_bombs, game->players[0].pos)) {
        game->players[0].vivo = 0;
    }
    if (colExplosion(game->players[0].bombs, game->players[0].num_bombs, game->players[1].pos) ||
        colExplosion(game->players[1].bombs, game->players[1].num_bombs, game->players[1].pos)) {
        game->players[1].vivo = 0;
    }
}

void DrawGame(Game *game, Placar* placar) {
    BeginDrawing();
    ClearBackground(DARKGRAY);

    drawPickup(game->pickups, game->total_pickups);
    draw_map(&game->map);
    
    draw_bomb(game->players[0].bombs, game->players[0].num_bombs);
    draw_bomb(game->players[1].bombs, game->players[1].num_bombs);

    DrawRectangleRec(game->players[0].pos, game->players[0].color);
    DrawRectangleRec(game->players[1].pos, game->players[1].color);

    DrawText(game->players[0].nome, 630, 5, 30, game->players[0].color);
    DrawText(game->players[1].nome, 630, 40, 30, game->players[1].color);

    drawEspecials(game);

    if (game->time < 120) {
        DrawText(
            TextFormat("%02d:%02d:%02d\n", 1 - (int)(game->time/60),
                                          59 - (int) game->time % 60,
                                          99 - (int)(game->time * 100) % 100),
            630, 100, 40, BLACK);
    } else {
        DrawText("TIMES UP!", 610, 100, 34, BLACK);
    }
    
    EndDrawing();
}

void gameLoop(Game* game, Placar* placar) {
    updateGame(game);
    DrawGame(game, placar);
}
