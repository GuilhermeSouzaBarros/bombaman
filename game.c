#include "game.h"
#include "bombas.h"
#include "maps.h"

#include "raylib.h"
#include "stdio.h"
#include "stdlib.h"

Game* initGame(Map map, char* p1_nome, char* p2_nome){
    Game* g = (Game*)malloc(sizeof(Game));
    g->map = map;
    initPlayer(&g->players[0], p1_nome, ORANGE, (Rectangle){44, 44, STD_SIZE_ENT, STD_SIZE_ENT});
    initPlayer(&g->players[1], p2_nome, WHITE, (Rectangle){524, 524, STD_SIZE_ENT, STD_SIZE_ENT});
    g->n_pickups = 0;
    g->start_time = GetTime();
    g->time = g->start_time - GetTime();
    return g;
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

void DrawGame(Game *g) {
    BeginDrawing();
    ClearBackground(DARKGRAY);
    draw_map(&g->map);
    draw_bomb(g->players[0].bombs, g->players[0].num_bombs);
    draw_bomb(g->players[1].bombs, g->players[1].num_bombs);
    drawPickup(g->pickups, g->n_pickups);
    DrawRectangleRec(g->players[0].pos, g->players[0].color);
    DrawRectangleRec(g->players[1].pos, g->players[1].color);

    DrawText(g->players[0].nome, 630, 5, 30, g->players[0].color);
    DrawText(g->players[1].nome, 630, 40, 30, g->players[1].color);

    if (g->time < 120) {
        DrawText(TextFormat("%02d:%02d:%02d\n", 1 - (int)(g->time/60), 60 - (int)g->time, 100 - (int)(g->time * 100) % 100),
            630, 100, 40, BLACK);
    } else {
        DrawText("TIMES UP!", 610, 100, 34, BLACK);
    }
    
    EndDrawing();
}

void gameLoop(Game* game) {
    updateGame(game);
    DrawGame(game);
}
