#include "menu.h"
#include "maps.h"

#include <stdlib.h>
#include <string.h>

Menu* initMenu(Placar* placar) {
    Menu* menu = (Menu*)malloc(sizeof(Menu));
    menu->text_input_1 = 0;
    menu->text_input_2 = 0;
    menu->p1_nome    = placar->p1_nome;
    menu->p1_qletras = strlen(menu->p1_nome);
    menu->p2_nome    = placar->p2_nome;
    menu->p2_qletras = strlen(menu->p2_nome);
    menu->screen     = placar->rematch;
    menu->game_start = 0;
    menu->map = 0;
    return menu;
}

EndMenu* initEndMenu(Game* game, Placar* placar) {
    EndMenu* endmenu = (EndMenu*)malloc(sizeof(EndMenu));
    endmenu->num_map = game->map.map_num;
    strcpy(endmenu->p1_nome, game->players[0].nome);
    strcpy(endmenu->p2_nome, game->players[1].nome);
    if (!WindowShouldClose()) {
        if (game->players[0].vivo) {
            placar->p1_wins++;
            endmenu->winner = 1;
            endmenu->winner_color = game->players[0].color;
        } else if (game->players[1].vivo) {
            placar->p2_wins++;
            endmenu->winner = 2;
            endmenu->winner_color = game->players[1].color;
        } else {
            placar->draws++;
            endmenu->winner = 0;
            endmenu->winner_color = LIGHTGRAY;
        }
    }
    endmenu->exit = 0;
    return endmenu;
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

void updateScreen1(Menu *menu, Placar* placar) {
    if (clickRec((Rectangle){75, 150, 300, 50})) {
        menu->text_input_1 = 1;
    }
    textoRec(menu->p1_nome, &menu->p1_qletras, (Rectangle){75, 150, 300, 50}, &menu->text_input_1);
    if (clickRec((Rectangle){425, 150, 300, 50})) {
        menu->text_input_2 = 1;
    }
    textoRec(menu->p2_nome, &menu->p2_qletras, (Rectangle){425, 150, 300, 50}, &menu->text_input_2);
    if (clickRec((Rectangle){588, 513, 200, 75}) && menu->p1_qletras && menu->p2_qletras) {
        menu->game_start = 1;
        return;
    }

    if (clickRec((Rectangle){100, 250, 250, 150})) {
        menu->map = 0;
        placar->next_map = 0;
    }

    if (clickRec((Rectangle){450, 250, 250, 150})) {
        menu->map = 1;
        placar->next_map = 1;
    }

    BeginDrawing();
    ClearBackground(DARKGRAY);
    
    DrawRectangleRec((Rectangle){75, 150, 300, 50}, WHITE);
    DrawRectangleLinesEx((Rectangle){75, 150, 300, 50}, 1, BLACK);
    DrawText(menu->p1_nome, 80, 158, 40, BLACK);
    
    DrawRectangleRec((Rectangle){425, 150, 300, 50}, WHITE);
    DrawRectangleLinesEx((Rectangle){425, 150, 300, 50}, 1, BLACK);
    DrawText(menu->p2_nome, 430, 158, 40, BLACK);

    DrawRectangleRec((Rectangle){100, 250, 250, 150}, ORANGE);
    if (!menu->map) {
        DrawRectangleLinesEx((Rectangle){100, 250, 250, 150}, 1, BLACK);
    }

    DrawRectangleRec((Rectangle){450, 250, 250, 150}, WHITE);
    if (menu->map) {
        DrawRectangleLinesEx((Rectangle){450, 250, 250, 150}, 1, BLACK);
    }

    if(menu->p1_qletras && menu->p2_qletras) {
        DrawRectangleRec((Rectangle){588, 513, 200, 75}, RED);
    } else {
        DrawRectangleRec((Rectangle){588, 513, 200, 75}, GRAY);
    }
    EndDrawing();
}

void menuLoop(Menu* menu, Placar* placar) {
    switch (menu->screen) {
        case 0:
            updateScreen0(menu);
            break;
        case 1:
            updateScreen1(menu, placar);
            break;
        default:
            menu->game_start = 1;
    }
}

void atualizaPlacar(EndMenu* endmenu, Placar* placar) {
    FILE* placar_file = fopen("placar.txt", "a");
    fputs(TextFormat("%s VS %s\n", placar->p1_nome, placar->p2_nome), placar_file);
    fputs(TextFormat("Vitórias de %s: %d\n", placar->p1_nome, placar->p1_wins), placar_file);
    fputs(TextFormat("Vitórias de %s: %d\n", placar->p2_nome, placar->p2_wins), placar_file);
    fputs(TextFormat("Empates: %d\n\n", placar->draws), placar_file);
    fclose(placar_file);
    *placar = (Placar){0, 0, 0, 0, 0, 0};
    endmenu->exit = 1;
}

void endMenuLoop(EndMenu* endmenu, Placar* placar) {
    if (clickRec((Rectangle){75, 400, 300, 100})) {
        placar->rematch = 1;
        placar->next_map = !endmenu->num_map;
        endmenu->exit = 1;
        return;
    }

    if ((WindowShouldClose() || clickRec((Rectangle){425, 400, 300, 100})) &&
        (placar->p1_wins || placar->p2_wins || placar->draws)) {
        atualizaPlacar(endmenu, placar);
        return;
    }

    BeginDrawing();
    ClearBackground(DARKGRAY);

    char text[20];
    Color color = {192, 192, 192, 255};
    if (endmenu->winner == 1) {
        strcpy(text, TextFormat("%s GANHOU!", endmenu->p1_nome));
        color = endmenu->winner_color;
    } else if (endmenu->winner == 2) {
        strcpy(text, TextFormat("%s GANHOU!", endmenu->p2_nome));
        color = endmenu->winner_color;
    } else {
        strcpy(text, "EMPATE");
    }

    DrawText(text, 400 - (MeasureText(text, 50)/2), 150, 50, color);

    DrawRectangleRec((Rectangle){75, 400, 300, 100}, BLUE);
    DrawText("REMATCH!", 225 - (MeasureText("REMATCH!", 50)/2), 425, 50, BLACK);

    DrawRectangleRec((Rectangle){425, 400, 300, 100}, RED);
    DrawText("Start Menu", 575 - (MeasureText("Start Menu", 50)/2), 425, 50, BLACK);

    EndDrawing();
}
