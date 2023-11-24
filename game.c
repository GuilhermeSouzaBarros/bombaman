#include "game.h"
#include <raylib.h>
#include "bombas.h"
#include "maps.h"

void InitGame(Game *g){
    g->curr_map = 0;
    g->num_maps = 10;
    initPlayer(&g->player1, ORANGE, (Rectangle){44, 44, STD_SIZE_ENT_X, STD_SIZE_ENT_Y});
    initPlayer(&g->player2, WHITE, (Rectangle){524, 524, STD_SIZE_ENT_X, STD_SIZE_ENT_Y});
    g->winner = 0;
    map0_setup(&g->maps[0]);
}

void UpdateGame(Game *g) {
    updatePlayersPos(&g->player1, &g->player2, &g->maps[g->curr_map]);
    updateBombs(&g->maps[g->curr_map],
        g->player1.pos, g->player1.bombs, g->player1.num_bombs, 
        g->player2.pos, g->player2.bombs, g->player2.num_bombs);
    colBombaPlayer(g->player1.bombs, g->player1.num_bombs, &g->player1);
    colBombaPlayer(g->player2.bombs, g->player2.num_bombs, &g->player1);
    colBombaPlayer(g->player1.bombs, g->player1.num_bombs, &g->player2);
    colBombaPlayer(g->player2.bombs, g->player2.num_bombs, &g->player2);
}

void DrawGame(Game *g) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawRectangle(0, 0, g->screenWidth, g->screenHeight, DARKGRAY);
    draw_map(&g->maps[g->curr_map]);
    draw_bomb(g->player1.bombs, g->player1.num_bombs);
    draw_bomb(g->player2.bombs, g->player2.num_bombs);
    DrawRectangleRec(g->player1.pos, g->player1.color);
    DrawRectangleRec(g->player2.pos, g->player2.color);
    DrawRectangleRec((Rectangle){600, 0, 200, 600}, BLACK);
    EndDrawing();
}

void UpdateDrawFrame(Game *g) {
    UpdateGame(g);
    DrawGame(g);
}
