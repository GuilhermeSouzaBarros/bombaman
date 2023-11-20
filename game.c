#include "game.h"
#include <raylib.h>
#include "bombas.h"
#include "maps.h"

void InitGame(Game *g){
    g->curr_map = 0;
    g->num_maps = 10;
    g->player.pos = (Rectangle) { 40, 40, STD_SIZE_ENT_X, STD_SIZE_ENT_Y};
    g->player.color = ORANGE;
    g->player.speed = 4;
    g->player.num_bombs = 5;
    g->player.draw_bomb = 0;
    for (int i = 0; i < 5; i++) {
        g->player.bombs[i].isActive = 0;
        g->player.bombs[i].distance = 120;
    }
    g->player.vivo = 1;
    map0_setup(&g->maps[0]);
}

void UpdateGame(Game *g) {
    update_player_pos(&g->player, &g->maps[g->curr_map]);
    update_bomb(&g->maps[g->curr_map], g->player.pos, g->player.bombs, g->player.num_bombs);
    colBombaPlayer(g->player.bombs, g->player.num_bombs, &g->player);
}

void DrawGame(Game *g) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawRectangle(0, 0, g->screenWidth, g->screenHeight, DARKGRAY);
    draw_map(&g->maps[g->curr_map]);
    draw_bomb(g->player.bombs, g->player.num_bombs);
    DrawRectangleRec(g->player.pos, g->player.color);
    DrawRectangleRec((Rectangle){600, 0, 200, 600}, BLACK);
    EndDrawing();
}

void UpdateDrawFrame(Game *g) {
    UpdateGame(g);
    DrawGame(g);
}
