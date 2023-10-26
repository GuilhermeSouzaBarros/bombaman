#include "funcoes.h"
#include "raylib.h"
#include <string.h>

void initGame(Game *g){
    Rectangle quadrado = {234, 184, 32, 32};
    Rectangle borda_w = {0, 0, 32, 400}, borda_e = {768, 0, 32, 400},
              borda_n = {0, 0, 800, 32}, borda_s = {0, 368, 800, 32};
    Game g_copy = {800, 400, quadrado, {borda_w, borda_e, borda_n, borda_s}};   
    *g = g_copy;
}

void colBorda (Game *g, int vel_x, int vel_y) {
    do {
        int col=0;
        g->quadrado.x += vel_x;
        for (int i = 0; i < 4; i++) {
            if (CheckCollisionRecs(g->quadrado, g->bordas[i])) {
                g->quadrado.x -= vel_x;
                if (vel_x > 0) vel_x--; else vel_x++;
                    col = 1;
                break;
            }
        }
        if (col) {
            col = 0;
            continue;
        }
        vel_x = 0;
    } while (vel_x);
    do {
        int col=0;
        g->quadrado.y += vel_y;
        for (int i = 0; i < 4; i++) {
            if (CheckCollisionRecs(g->quadrado, g->bordas[i])) {
                g->quadrado.y -= vel_y;
                if (vel_y > 0) vel_y--; else vel_y++;
                    col = 1;
                break;
            }
        }
        if (col) {
            col = 0;
            continue;
        }
        vel_y = 0;
    } while (vel_y);
}

void updateGame(Game *g) {
    int vel_x = 0, vel_y = 0;
    if (IsKeyDown(KEY_RIGHT)) vel_x += 4;
    if (IsKeyDown(KEY_LEFT)) vel_x -= 4;
    if (IsKeyDown(KEY_UP)) vel_y -= 4;
    if (IsKeyDown(KEY_DOWN)) vel_y += 4;
    colBorda(g, vel_x, vel_y);
}

void drawGame(Game *g) {
    BeginDrawing();
    ClearBackground (GRAY);
    DrawRectangleRec(g->quadrado, WHITE );
    for (int i = 0; i < 4; i++) {
        DrawRectangleRec(g->bordas[i], BLUE);
    }
    EndDrawing();
}