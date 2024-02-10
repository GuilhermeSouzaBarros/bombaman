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

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "SUPER BOMBAMAN");
    SetTargetFPS(60);
    SetExitKey(KEY_DELETE);

    Image icon = LoadImage("images/bob-omb-icon.png");
    SetWindowIcon(icon);

    Font font = LoadFont("font/setback.png");
    Placar placar = {0, 0, 0, 0, 0, 0, 0};

    InitAudioDevice();

    while(!WindowShouldClose()) {
        Menu* menu = initMenu(&placar, &font);
        while (!placar.rematch && !menu->game_start && !WindowShouldClose()) {
            menuLoop(menu, &placar);
        }

        Game* game = initGame(placar.next_map, menu->p1_nome, menu->p2_nome, &font);
        
        freeMenu(menu);
        
        while (!(game->exit_game || WindowShouldClose())) {
            gameLoop(game);
        }

        EndMenu* endmenu = initEndMenu(game, &placar, &font);
        
        freeGame(game);
        
        while(!endmenu->exit && !IsKeyDown(KEY_ENTER) && !WindowShouldClose()) {
            endMenuLoop(endmenu, &placar);
        }
        
        if (WindowShouldClose() && (placar.p1_wins || placar.p2_wins || placar.draws))
            atualizaPlacar(endmenu, &placar);
        
        freeEndMenu(endmenu);
    }

    UnloadImage(icon);
    UnloadFont(font);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}
