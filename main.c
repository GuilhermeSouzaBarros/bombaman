#include "raylib.h"
#include "funcoes.h"

int main () {
    Game g;
    initGame(&g);
    InitWindow(g.largura, g.altura, "Jogo");
    SetTargetFPS(75);
    while (!WindowShouldClose()){
        updateGame(&g);
        drawGame(&g);
    }
    CloseWindow();
}