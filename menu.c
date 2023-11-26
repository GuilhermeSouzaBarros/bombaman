#include "menu.h"
#include "maps.h"

#include <stdlib.h>

Menu* initMenu() {
    Menu* menu = (Menu*)malloc(sizeof(Menu));
    menu->p1_nome[0] = 0;
    menu->p1_qletras = 0;
    menu->p2_nome[0] = 0;
    menu->p2_qletras = 0;
    menu->screen     = 0;
    menu->game_start = 0;
    return menu;
}

void updateScreen0(Menu* menu) {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){200, 450, 400, 60})) {
            menu->screen = 1;
            menuLoop(menu);
            return;
        }
    }
    BeginDrawing();
    ClearBackground(DARKGRAY);
    DrawRectangleRec((Rectangle){200, 450, 400, 60}, RED);
    DrawText("START BOMBA", 206, 455, 50, BLACK);
    EndDrawing();
}

void updateScreen1(Menu *menu) {
    int letra = 0;
    int inserindo_texto = CheckCollisionPointRec(GetMousePosition(), (Rectangle){50, 200, 300, 50});
    if (inserindo_texto) {
        do {
            letra = GetCharPressed();
            if (letra >= 32 && letra <= 125 && menu->p1_qletras < 9) {
                menu->p1_nome[menu->p1_qletras] = (char)letra;
                menu->p1_nome[++menu->p1_qletras] = 0;
            }
        } while(letra);
    }

    if (IsKeyPressed(KEY_BACKSPACE) && menu->p1_qletras) {
        menu->p1_nome[--menu->p1_qletras] = 0;
    }

    if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){588, 513, 200, 75}) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        menu->game_start = 1;
        return;
    }

    BeginDrawing();
    ClearBackground(DARKGRAY);
    
    DrawRectangleRec((Rectangle){50, 200, 300, 50}, WHITE);
    DrawRectangleLinesEx((Rectangle){50, 200, 300, 50}, 1, BLACK);
    DrawText(menu->p1_nome, 55, 208, 40, BLACK);
    DrawRectangleRec((Rectangle){588, 513, 200, 75}, RED);
    EndDrawing();
}

void menuLoop(Menu* menu) {
    switch (menu->screen) {
        case 0:
            updateScreen0(menu);
            break;
        case 1:
            updateScreen1(menu);
            break;
        default:
            menu->game_start = 1;
    }
}
