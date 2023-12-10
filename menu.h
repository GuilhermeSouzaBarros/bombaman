#include "raylib.h"
#include "stdio.h"

#include "structs.h"

#ifndef MENU
#define MENU

void drawTextBox(Font* font, char* text, Vector2 pos, int font_size, int spacing, Color color);

Menu* initMenu(Placar* placar, Font* font);
void freeMenu(Menu* menu);

EndMenu* initEndMenu(Game* game, Placar* placar, Font* font);
void freeEndMenu(EndMenu* endmenu);

void menuLoop(Menu* menu, Placar* placar);
void endMenuLoop(EndMenu* endmenu, Placar* placar);

void atualizaPlacar(EndMenu* endmenu, Placar* placar);

#endif