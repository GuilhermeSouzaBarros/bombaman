#include "menu.h"
#include "maps.h"

#include <stdlib.h>
#include <string.h>

Menu* initMenu(Placar* placar, Font* font) {
    Menu* menu = (Menu*)malloc(sizeof(Menu));
    if (rand() % 2) {
        menu->background = LoadTexture("images/hotlands.png");
        menu->pos_sprite = (Rectangle){0, 0, 1200, 900};
    } else {
        menu->background = LoadTexture("images/isaac.png");
        menu->pos_sprite = (Rectangle){0, 0, 800, 600};
    }
    menu->text_input_1 = 0;
    menu->text_input_2 = 0;
    menu->p1_nome    = placar->p1_nome;
    menu->p1_qletras = strlen(menu->p1_nome);
    menu->p2_nome    = placar->p2_nome;
    menu->p2_qletras = strlen(menu->p2_nome);
    menu->screen     = placar->rematch;
    menu->game_start = 0;
    menu->map = 0;
    menu->font = font;
    return menu;
}

void freeMenu(Menu* menu) {
    UnloadTexture(menu->background);
    free(menu);
}

EndMenu* initEndMenu(Game* game, Placar* placar, Font* font) {
    EndMenu* endmenu = (EndMenu*)malloc(sizeof(EndMenu));
    endmenu->num_map = game->map.map_num;
    strcpy(endmenu->p1_nome, game->players[0].nome);
    strcpy(endmenu->p2_nome, game->players[1].nome);
    if (!WindowShouldClose()) {
        if (game->players[0].vivo) {
            endmenu->music = LoadMusicStream("sounds/sans.mp3");
            SetMusicVolume(endmenu->music, 0.6);
            PlayMusicStream(endmenu->music);
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

void freeEndMenu(EndMenu* endmenu) {
    UnloadMusicStream(endmenu->music);
    free(endmenu);
}

int clickRec(Rectangle rec) {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
        CheckCollisionPointRec(GetMousePosition(), rec)) {
        return 1;
    }
    return 0;
}

void updateScreen0(Menu* menu) {
    BeginDrawing();
    DrawTexturePro(menu->background, menu->pos_sprite,
        (Rectangle){0, 0, 800, 600}, (Vector2){0, 0}, 0, WHITE);

    Vector2 size = MeasureTextEx(*menu->font, "SUPER", 50, 5);
    DrawRectangle(224 - size.x/2, 100 - size.y/2, size.x + 8, size.y, (Color){0, 0, 0, 192});
    DrawTextEx(*menu->font, "SUPER", (Vector2){228 - size.x / 2, 100 - size.y / 2}, 50, 5, (Color){80, 220, 220, 255});

    size = MeasureTextEx(*menu->font, "BOMBA", 150, 15);
    DrawRectangle(395 - size.x/2, 195 - size.y/2, size.x + 16, size.y - 23, (Color){0, 0, 0, 192});
    DrawTextEx(*menu->font, "BOMBA", (Vector2){400 - size.x / 2, 190 - size.y / 2}, 150, 15, GOLD);

    size = MeasureTextEx(*menu->font, "MAN", 150, 15);
    DrawRectangle(498 - size.x/2, 320 - size.y/2, size.x + 16, size.y - 19, (Color){0, 0, 0, 192});
    DrawTextEx(*menu->font, "MAN", (Vector2){503 - size.x / 2, 320 - size.y / 2}, 150, 15, GOLD);

    size = MeasureTextEx(*menu->font, "START BOMBA", 50, 5);
    DrawRectangle(195, 445, 410, 70, (Color){0, 0, 0, 255});
    DrawRectangle(200, 450, 400, 60, RED);
    DrawTextEx(*menu->font, "START BOMBA", (Vector2){220, 455}, 50, 5, WHITE);

    EndDrawing();

    if (clickRec((Rectangle){200, 450, 400, 60})) {
        menu->screen = 1;
        return;
    }
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
    BeginDrawing();
    DrawTexturePro(menu->background, menu->pos_sprite,
        (Rectangle){0, 0, 800, 600}, (Vector2){0, 0}, 0, WHITE);
    
    Vector2 size = MeasureTextEx(*menu->font, "SEUS APELIDOS", 50, 5);
    size.x = 400 - size.x/2;
    size.y = 100 - size.y/2;
    DrawTextEx(*menu->font, "SEUS APELIDOS", size, 50, 5, WHITE);

    
    DrawRectangleRec((Rectangle){75, 150, 300, 50}, WHITE);
    DrawRectangleLinesEx((Rectangle){75, 150, 300, 50}, 1, BLACK);
    DrawText(menu->p1_nome, 80, 158, 40, (Color){128, 228, 228, 255});
    
    DrawRectangleRec((Rectangle){425, 150, 300, 50}, WHITE);
    DrawRectangleLinesEx((Rectangle){425, 150, 300, 50}, 1, BLACK);
    DrawText(menu->p2_nome, 430, 158, 40, (Color){206, 161, 154, 255});

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
    UpdateMusicStream(endmenu->music);
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
}
