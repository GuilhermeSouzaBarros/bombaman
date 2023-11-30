#include "game.h"
#include "bombas.h"
#include "maps.h"

#include "raylib.h"
#include "stdlib.h"

Game* initGame(Map map, char* p1_nome, char* p2_nome){
    Game* g = (Game*)malloc(sizeof(Game));
    g->map = map;
    initPlayer(&g->player1, p1_nome, ORANGE, (Rectangle){44, 44, STD_SIZE_ENT_X, STD_SIZE_ENT_Y});
    initPlayer(&g->player2, p2_nome, WHITE, (Rectangle){524, 524, STD_SIZE_ENT_X, STD_SIZE_ENT_Y});
    g->n_pickups = 0;
    return g;
}

void updateGame(Game *g) {
    updatePlayersPos(&g->player1, &g->player2, &g->map);
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

    EndDrawing();
}

void gameLoop(Game* game) {
    updateGame(game);
    DrawGame(game);
}
