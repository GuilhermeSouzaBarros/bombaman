#include "game.h"
#include "bombas.h"
#include "maps.h"

#include "raylib.h"
#include "stdio.h"
#include "stdlib.h"

Game* initGame(Map map, char* p1_nome, char* p2_nome){
    Game* g = (Game*)malloc(sizeof(Game));
    g->map = map;
    initPlayer(&g->player1, p1_nome, ORANGE, (Rectangle){44, 44, STD_SIZE_ENT_X, STD_SIZE_ENT_Y});
    initPlayer(&g->player2, p2_nome, WHITE, (Rectangle){524, 524, STD_SIZE_ENT_X, STD_SIZE_ENT_Y});
    g->n_pickups = 0;
    g->time = GetTime();
    return g;
}

int timesUp(double time) {
    if (GetTime() - time > 120) {
        return 1;
    } else {
        return 0;
    }
}

void updateGame(Game *g) {
    if (!timesUp(g->time)) {
        updatePlayersPos(&g->player1, &g->player2, &g->map);
    } else {
        for (int i = 0; i < g->player1.num_bombs; i++) {
            g->player1.bombs[i].fastExplode = 1;
        }
        for (int i = 0; i < g->player2.num_bombs; i++) {
            g->player2.bombs[i].fastExplode = 1;
        }
        int cabou = 1;
        for (int i = 0; i < 5; i ++) {
            if (g->player1.bombs[i].isActive || g->player2.bombs[i].isActive) {
                cabou = 0;
            }
        }
        if (cabou) {
            g->player1.vivo = 0;
            g->player2.vivo = 0;
        }
    }
    colPlayerPickups(&g->player1, g->pickups, &g->n_pickups);
    colPlayerPickups(&g->player2, g->pickups, &g->n_pickups);
    updateBombs(&g->map, g->pickups, &g->n_pickups,
        g->player1.pos, g->player1.bombs, g->player1.num_bombs, 
        g->player2.pos, g->player2.bombs, g->player2.num_bombs);
    colExplosionPlayer(g->player1.bombs, g->player1.num_bombs, &g->player1);
    colExplosionPlayer(g->player2.bombs, g->player2.num_bombs, &g->player1);
    colExplosionPlayer(g->player1.bombs, g->player1.num_bombs, &g->player2);
    colExplosionPlayer(g->player2.bombs, g->player2.num_bombs, &g->player2);
}

void DrawGame(Game *g) {
    BeginDrawing();
    ClearBackground(DARKGRAY);
    draw_map(&g->map);
    draw_bomb(g->player1.bombs, g->player1.num_bombs);
    draw_bomb(g->player2.bombs, g->player2.num_bombs);
    drawPickup(g->pickups, g->n_pickups);
    DrawRectangleRec(g->player1.pos, g->player1.color);
    DrawRectangleRec(g->player2.pos, g->player2.color);

    DrawText(g->player1.nome, 630, 5, 30, g->player1.color);
    DrawText(g->player2.nome, 630, 40, 30, g->player2.color);

    if (!timesUp(g->time)) {
        double time = GetTime();
        DrawText(TextFormat("%02d:%02d:%02d\n", 1 - (int)(time/60), 60 - (int)time, 100 - (int)(time * 100) % 100),
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
