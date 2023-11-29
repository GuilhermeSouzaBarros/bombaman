#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <unistd.h>

#include "menu.h"
#include "game.h"
#include "player.h"
#include "bombas.h"
#include "maps.h"

int main(void) {
    srand(time(NULL));
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "BOMBAMAN");
    SetTargetFPS(60);
    SetExitKey(KEY_DELETE);
    while(!WindowShouldClose()) {

        Menu* menu = initMenu();

        while (!menu->game_start && !WindowShouldClose()) {
            menuLoop(menu);
        }
        Game* game = initGame(mapSetup(0), menu->p1_nome, menu->p2_nome);
        free(menu);

        while (game->player1.vivo && game->player2.vivo && !WindowShouldClose()) {
            gameLoop(game);
        }
        free(game);

        while(!IsKeyDown(KEY_ENTER) && !WindowShouldClose()) {
            BeginDrawing();
            ClearBackground(RAYWHITE);
            if(game->player1.vivo) {
                DrawText("GANHASTE JOGADOR 1", GetScreenWidth()/2 - MeasureText("GANHASTE JOGADOR 1", 20)/2, GetScreenHeight()/2 - 50, 20, BLACK);
            } else if (game->player2.vivo) {
                DrawText("GANHASTE JOGADOR 2", GetScreenWidth()/2 - MeasureText("GANHASTE JOGADOR 2", 20)/2, GetScreenHeight()/2 - 50, 20, BLACK);
            } else {
                DrawText("EMPATASTES", GetScreenWidth()/2 - MeasureText("EMPATASTES", 20)/2, GetScreenHeight()/2 - 50, 20, BLACK);
            }
            EndDrawing();
        }
    }
    return 0;
}
