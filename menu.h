#include "raylib.h"

#ifndef MENU
#define MENU

typedef struct Menu {
    int screen;
    int game_start;
    int text_input_1;
    int text_input_2;
    char p1_nome[10];
    int p1_qletras;
    char p2_nome[10];
    int p2_qletras;
} Menu;

Menu* initMenu();
void menuLoop(Menu* menu);

#endif