#ifndef funcoes
#define funcoes

#include "raylib.h"
typedef struct {
    int largura;
    int altura;
    Rectangle quadrado;
    Rectangle bordas[4];
} Game;

void initGame(Game *g);
void updateGame(Game *g);
void drawGame(Game *g);
#endif