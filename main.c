#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <unistd.h>

#include "game.h"
#include "player.h"
#include "bombas.h"
#include "maps.h"

int main(void)
{
    Game game;
    game.screenWidth = 800;
    game.screenHeight = 600;

    InitWindow(game.screenWidth, game.screenHeight, "BOMBAMAN");
    SetTargetFPS(60);
    
    InitGame(&game);

    while (!WindowShouldClose()) {
        UpdateDrawFrame(&game);
        if(!game.player1.vivo || !game.player2.vivo) {
            sleep(2);
            break;
        }
    }

    while(!IsKeyDown(KEY_ENTER)) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        if(game.player1.vivo) {
            DrawText("GANHASTE JOGADOR 1", GetScreenWidth()/2 - MeasureText("GANHASTE JOGADOR 1", 20)/2, GetScreenHeight()/2 - 50, 20, BLACK);
        } else if (game.player2.vivo) {
            DrawText("GANHASTE JOGADOR 2", GetScreenWidth()/2 - MeasureText("GANHASTE JOGADOR 2", 20)/2, GetScreenHeight()/2 - 50, 20, BLACK);
        } else {
            DrawText("EMPATASTES", GetScreenWidth()/2 - MeasureText("EMPATASTES", 20)/2, GetScreenHeight()/2 - 50, 20, BLACK);
        }
        EndDrawing();
    }
    return 0;
}
