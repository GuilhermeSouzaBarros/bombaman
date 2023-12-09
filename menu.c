#include "menu.h"
#include "maps.h"

#include <stdlib.h>
#include <string.h>

Menu* initMenu(Placar* placar, Font* font) {
    Menu* menu = (Menu*)malloc(sizeof(Menu));
    if (rand() % 2) {
        menu->background = LoadTexture("images/hotlands.png");
        menu->pos_sprite = (Rectangle){0, 0, 1200, 900};
        menu->music = LoadMusicStream("sounds/Undertale OST - Start Menu Theme.mp3");
    } else {
        menu->background = LoadTexture("images/isaac.png");
        menu->pos_sprite = (Rectangle){0, 0, 800, 600};
        menu->music = LoadMusicStream("sounds/The Binding of Isaac OST - Title Theme.mp3");
    }
    PlayMusicStream(menu->music);
    menu->map_icons[0] = LoadTexture("sprites/Undertale-Logo.png");
    menu->map_icons[1] = LoadTexture("sprites/isaac_logo.png");
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
    menu->click = LoadSound("sounds/click-button.mp3");
    return menu;
}

void freeMenu(Menu* menu) {
    UnloadTexture(menu->background);
    for (int i = 0; i < 2; i++) {
        UnloadTexture(menu->map_icons[i]);
    }
    UnloadMusicStream(menu->music);
    UnloadSound(menu->click);
    free(menu);
}

EndMenu* initEndMenu(Game* game, Placar* placar, Font* font) {
    EndMenu* endmenu = (EndMenu*)malloc(sizeof(EndMenu));
    endmenu->num_map = game->map.map_num;
    strcpy(endmenu->p1_nome, game->players[0].nome);
    strcpy(endmenu->p2_nome, game->players[1].nome);
    endmenu->winner = 0;
    if (!WindowShouldClose()) {
        if (game->players[0].vivo) {
            endmenu->winner_sprite = LoadTexture("sprites/sans.png");
            endmenu->music = LoadMusicStream("sounds/sans.mp3");
            SetMusicVolume(endmenu->music, 0.6);
            PlayMusicStream(endmenu->music);
            placar->p1_wins++;
            endmenu->winner = 1;
            endmenu->winner_color = (Color){128, 228, 228, 255};
        } else if (game->players[1].vivo) {
            endmenu->winner_sprite = LoadTexture("sprites/isaac-thumbsup.png");
            endmenu->music = LoadMusicStream("sounds/specialist.mp3");
            SetMusicVolume(endmenu->music, 0.6);
            PlayMusicStream(endmenu->music);
            placar->p2_wins++;
            endmenu->winner = 2;
            endmenu->winner_color = (Color){206, 161, 154, 255};
        } else {
            placar->draws++;
            endmenu->winner_color = LIGHTGRAY;
        }
    }
    endmenu->exit = 0;
    endmenu->font = font;
    endmenu->placar = *placar;
    endmenu->click = LoadSound("sounds/click-button.mp3");
    return endmenu;
}

void freeEndMenu(EndMenu* endmenu) {
    if (endmenu->winner) {
        UnloadMusicStream(endmenu->music);
        UnloadTexture(endmenu->winner_sprite);
    }
    UnloadSound(endmenu->click);
    free(endmenu);
}

int clickRec(Rectangle rec) {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
        CheckCollisionPointRec(GetMousePosition(), rec)) {
        return 1;
    }
    return 0;
}

int hoverRec(Rectangle rec) {
    if (CheckCollisionPointRec(GetMousePosition(), rec)) {
        return 1;
    }
    return 0;
}

void drawTextBox(Font* font, char* text, Vector2 pos, int font_size, int spacing, Color color) {
    Vector2 size = MeasureTextEx(*font, text, font_size, spacing);
    DrawRectangle(pos.x - size.x/2 - 4, pos.y - size.y/2 + 8, size.x + 4, size.y - 16, (Color){0, 0, 0, 128});
    DrawTextEx(*font, text, (Vector2){pos.x - size.x / 2, pos.y - size.y / 2}, font_size, spacing, color);
}

void drawButtonWithText(Font* font, char* text, Rectangle pos, int font_size, int spacing, Color color) {
    if (hoverRec(pos)) {
        color.r /= 2;
        color.g /= 2;
        color.b /= 2;
    }
        DrawRectangle(pos.x - 4, pos.y - 4, pos.width + 8, pos.height + 8, (Color){0, 0, 0, 192});
        DrawRectangle(pos.x - 2, pos.y - 2, pos.width + 4, pos.height + 4, color);
        DrawTextEx(*font, text, (Vector2){pos.x, pos.y}, font_size, spacing, BLACK);
}

void updateScreen0(Menu* menu) {
    BeginDrawing();
    DrawTexturePro(menu->background, menu->pos_sprite,
        (Rectangle){0, 0, 800, 600}, (Vector2){0, 0}, 0, WHITE);

    drawTextBox(menu->font, "SUPER", (Vector2){228, 100},  50,  5, (Color){80, 220, 220, 255});
    drawTextBox(menu->font, "BOMBA", (Vector2){400, 190}, 150, 15, GOLD);
    drawTextBox(menu->font, "MAN"  , (Vector2){503, 320}, 150, 15, GOLD);

    Vector2 size = MeasureTextEx(*menu->font, "START BOMBA", 50, 5);
    Rectangle start_button = {400 - size.x / 2, 450 - size.y / 2, size.x, size.y};
    drawButtonWithText(menu->font, "START BOMBA", start_button, 50, 5, RED);

    EndDrawing();

    if (clickRec(start_button)) {
        PlaySound(menu->click);
        menu->screen = 1;
        return;
    }
}

void textoRec(char* texto, int* n, Rectangle rec, int* inserindo_texto, Sound* sound) {
    if (*inserindo_texto && CheckCollisionPointRec(GetMousePosition(), rec)) {
        int letra = 0;
        if (inserindo_texto) {
            do {
                letra = GetCharPressed();
                if (letra >= 32 && letra <= 125 && *n < 9) {
                    texto[*n] = (char)letra;
                    texto[++*n] = 0;
                    PlaySound(*sound);
                }
            } while(letra);
            if (IsKeyPressed(KEY_BACKSPACE) && *n) {
                texto[--*n] = 0;
                PlaySound(*sound);
            }
        }
    } else {
        *inserindo_texto = 0;
    }
}

void drawInputTextBox(Font* font, char* text, Rectangle pos, int font_size, int spacing, Color color, int active) {
    Color Background = {32, 32, 32, 255};
    if (active) {
        Background = (Color){128, 128, 128, 255};
    } else {
        if (CheckCollisionPointRec(GetMousePosition(), pos)) {
            Background = (Color){64, 64, 64, 255};
        }
    }
    DrawRectangleRec(pos, Background);
    DrawRectangleLinesEx(pos, 1, WHITE);
    DrawTextEx(*font, text, (Vector2){pos.x + 5, pos.y + 5}, font_size, spacing, color);
}

void updateScreen1(Menu *menu, Placar* placar) {
    BeginDrawing();
    DrawTexturePro(menu->background, menu->pos_sprite,
        (Rectangle){0, 0, 800, 600}, (Vector2){0, 0}, 0, WHITE);

    drawTextBox(menu->font, "APELIODS", (Vector2){400, 100}, 50, 5, WHITE);
    
    drawInputTextBox(menu->font, menu->p1_nome, (Rectangle){75, 150, 300, 50}, 40, 4,
        (Color){128, 228, 228, 255}, menu->text_input_1);
    drawInputTextBox(menu->font, menu->p2_nome, (Rectangle){425, 150, 300, 50}, 40, 4,
        (Color){206, 161, 154, 255}, menu->text_input_2);

    DrawRectangle(100, 250, 250, 150, BLACK);
    DrawTexturePro(menu->map_icons[0], (Rectangle){0, 0, 3840, 2160}, (Rectangle){100, 250, 250, 150},
        (Vector2){0, 0}, 0, WHITE);
    if (!menu->map) {
        DrawRectangleLinesEx((Rectangle){100, 250, 250, 150}, 2, WHITE);
    }

    DrawRectangle(450, 250, 250, 150, WHITE);
    DrawTexturePro(menu->map_icons[1], (Rectangle){60, 96, 380, 108}, (Rectangle){450, 250, 250, 80},
        (Vector2){0, -35}, 0, WHITE);
    if (menu->map) {
        DrawRectangleLinesEx((Rectangle){450, 250, 250, 150}, 2, (Color){173, 139, 90, 255});
    }

    char start_button_text[9];
    Color start_button_color;

    if(menu->p1_qletras && menu->p2_qletras) {
        strcpy(start_button_text, "LESS GO");
        start_button_color = RED;
    } else {
        strcpy(start_button_text, "Apelidos");
        start_button_color = GRAY;
    }
    Vector2 size = MeasureTextEx(*menu->font, start_button_text, 50, 5);
    Rectangle start_match = {650 - size.x / 2, 550 - size.y / 2, size.x, size.y};
    drawButtonWithText(menu->font, start_button_text, start_match, 50, 5, start_button_color);

    EndDrawing();

    if (clickRec((Rectangle){75, 150, 300, 50})) {
        PlaySound(menu->click);
        menu->text_input_1 = 1;
    }
    textoRec(menu->p1_nome, &menu->p1_qletras, (Rectangle){75, 150, 300, 50}, &menu->text_input_1, &menu->click);

    if (clickRec((Rectangle){425, 150, 300, 50})) {
        PlaySound(menu->click);
        menu->text_input_2 = 1;
    }
    textoRec(menu->p2_nome, &menu->p2_qletras, (Rectangle){425, 150, 300, 50}, &menu->text_input_2, &menu->click);

    if (clickRec(start_match) && menu->p1_qletras && menu->p2_qletras) {
        menu->game_start = 1;
        return;
    }

    if (clickRec((Rectangle){100, 250, 250, 150})) {
        PlaySound(menu->click);
        menu->map = 0;
        placar->next_map = 0;
    }

    if (clickRec((Rectangle){450, 250, 250, 150})) {
        PlaySound(menu->click);
        menu->map = 1;
        placar->next_map = 1;
    }
}

void menuLoop(Menu* menu, Placar* placar) {
    UpdateMusicStream(menu->music);
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

void drawPlacar(EndMenu* endmenu) {
    drawTextBox(endmenu->font, endmenu->p1_nome, (Vector2){200, 150}, 40, 4, (Color){128, 228, 228, 255});
    char number[5];
    strcpy(number, TextFormat("%d", endmenu->placar.p1_wins));
    drawTextBox(endmenu->font, number, (Vector2){200, 250}, 100, 10, WHITE);

    strcpy(number, TextFormat("%d", endmenu->placar.draws));
    drawTextBox(endmenu->font, "EMPATES", (Vector2){400, 115}, 30, 3, (Color){128, 128, 128, 255});
    drawTextBox(endmenu->font, number, (Vector2){400, 165}, 60, 6, WHITE);

    strcpy(number, TextFormat("%d", endmenu->placar.p2_wins));
    drawTextBox(endmenu->font, endmenu->p2_nome, (Vector2){600, 150}, 40, 5, (Color){206, 161, 154, 255});
    drawTextBox(endmenu->font, number, (Vector2){600, 250}, 100, 10, WHITE);
}

void endMenuLoop(EndMenu* endmenu, Placar* placar) {
    UpdateMusicStream(endmenu->music);
    BeginDrawing();
    ClearBackground(DARKGRAY);
    if (endmenu->winner == 1) {
        ClearBackground(BLACK);
        DrawTexturePro(endmenu->winner_sprite, (Rectangle){0, 6, 200, 200}, (Rectangle){300, 200, 200, 200},
        (Vector2){0, 0}, 0, WHITE);
    } 
    if (endmenu->winner == 2) {
        ClearBackground((Color){32, 59, 78, 255});
        DrawTexturePro(endmenu->winner_sprite, (Rectangle){124, 135, 752, 730}, (Rectangle){300, 200, 200, 200},
        (Vector2){0, 0}, 0, WHITE);
    }

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

    drawTextBox(endmenu->font, text, (Vector2){400, 75}, 50, 5, color);

    drawPlacar(endmenu);

    Vector2 size = MeasureTextEx(*endmenu->font, "REMATCH!", 50, 5);
    size.x += 4;
    size.y *= 0.9;
    Rectangle rematch_button = {200 - size.x / 2, 450 - size.y / 2, size.x, size.y};
    drawButtonWithText(endmenu->font, "REMATCH!", rematch_button, 50, 5, (Color){32, 255, 255, 255});

    size = MeasureTextEx(*endmenu->font, "Start Menu", 50, 4);
    size.x += 4;
    size.y *= 0.9;
    Rectangle main_menu_button = {600 - size.x / 2, 450 - size.y / 2, size.x, size.y};
    drawButtonWithText(endmenu->font, "Start Menu", main_menu_button, 50, 4, RED);
    

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
