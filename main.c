#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <unistd.h>

#include "structs.h"
#include "menu.h"
#include "game.h"
#include "player.h"
#include "bombas.h"
#include "maps.h"

int main(void) {
    srand(time(NULL));
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "BOMBAMAN");
    SetTargetFPS(60);
    SetExitKey(KEY_DELETE);
    Placar placar = {0, 0, 0, 0, 0, 0};
    while(!WindowShouldClose()) {
        Menu* menu = initMenu(&placar);
        while (!placar.rematch && !menu->game_start && !WindowShouldClose()) {
            menuLoop(menu, &placar);
        }
        
        Game* game = initGame(0, menu->p1_nome, menu->p2_nome);
        free(menu);

        while (game->players[0].vivo && game->players[1].vivo && !WindowShouldClose()) {
            gameLoop(game, &placar);
        }

        EndMenu* endmenu = initEndMenu(game, &placar);
        free(game);

        while(!endmenu->exit && !IsKeyDown(KEY_ENTER) && !WindowShouldClose()) {
            endMenuLoop(endmenu, &placar);
        }
        free(endmenu);
    }
    return 0;
}
