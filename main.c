#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
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
        if(!game.player.vivo) break;
    }

    while(!IsKeyDown(KEY_ENTER)) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("PERDESTE", GetScreenWidth()/2 - MeasureText("PERDESTE", 20)/2, GetScreenHeight()/2 - 50, 20, BLACK);
        EndDrawing();
    }
    return 0;
}
