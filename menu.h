#include "raylib.h"
#include "stdio.h"

#include "structs.h"

#ifndef MENU
#define MENU

Menu* initMenu(Placar* placar);
EndMenu* initEndMenu(Game* game, Placar* placar);
void menuLoop(Menu* menu, Placar* placar);
void endMenuLoop(EndMenu* endmenu, Placar* placar);

void atualizaPlacar(EndMenu* endmenu, Placar* placar);

#endif