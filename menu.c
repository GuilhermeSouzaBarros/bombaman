#include "menu.h"
#include "maps.h"

#include <stdlib.h>

Menu* initMenu() {
    Menu* menu = (Menu*)malloc(sizeof(Menu));
    menu->text_input_1 = 0;
    menu->text_input_2 = 0;
    menu->p1_nome[0] = 0;
    menu->p1_qletras = 0;
    menu->p2_nome[0] = 0;
    menu->p2_qletras = 0;
    menu->screen     = 0;
    menu->game_start = 0;
    return menu;
}

int clickRec(Rectangle rec) {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
        CheckCollisionPointRec(GetMousePosition(), rec)) {
        return 1;
    }
    return 0;
}

void updateScreen0(Menu* menu) {
    if (clickRec((Rectangle){200, 450, 400, 60})) {
        menu->screen = 1;
        menuLoop(menu);
        return;
    }
    BeginDrawing();
    ClearBackground(DARKGRAY);
    DrawRectangleRec((Rectangle){200, 450, 400, 60}, RED);
    DrawText("START BOMBA", 206, 455, 50, BLACK);
    EndDrawing();
}

void textoRec(char* texto, int* n, Rectangle rec, int* inserindo_texto) {
    if (*inserindo_texto && CheckCollisionPointRec(GetMousePosition(), rec)) {
        int letra = 0;
        if (inserindo_texto) {
            do {
                letra = GetCharPressed();
                if (letra >= 32 && letra <= 125 && *n < 9) {
                    texto[*n] = (char)letra;
                    texto[++*n] = 0;
                }
            } while(letra);
            if (IsKeyPressed(KEY_BACKSPACE) && *n) {
                texto[--*n] = 0;
            }
        }
    } else {
        *inserindo_texto = 0;
    }
}

void updateScreen1(Menu *menu) {
    if (clickRec((Rectangle){50, 200, 300, 50})) {
        menu->text_input_1 = 1;
    }
    textoRec(menu->p1_nome, &menu->p1_qletras, (Rectangle){50, 200, 300, 50}, &menu->text_input_1);
    if (clickRec((Rectangle){50, 300, 300, 50})) {
        menu->text_input_2 = 1;
    }
    textoRec(menu->p2_nome, &menu->p2_qletras, (Rectangle){50, 300, 300, 50}, &menu->text_input_2);
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
    
    DrawRectangleRec((Rectangle){50, 300, 300, 50}, WHITE);
    DrawRectangleLinesEx((Rectangle){50, 300, 300, 50}, 1, BLACK);
    DrawText(menu->p2_nome, 55, 308, 40, BLACK);

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
