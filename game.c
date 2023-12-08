#include "game.h"
#include "bombas.h"
#include "maps.h"

#include "raylib.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"

Game* initGame(int map, char* p1_nome, char* p2_nome, Font* font){
    Game* g = (Game*)malloc(sizeof(Game));
    g->pickups_sprite = LoadTexture("sprites/pickups.png");
    g->total_pickups = 0;
    g->start_time = GetTime();
    g->time = g->start_time - GetTime();

    g->music = LoadMusicStream("sounds/Undertale OST - Another Medium.mp3");
    SetMusicVolume(g->music, 0.6);
    SeekMusicStream(g->music, 105);
    PlayMusicStream(g->music);

    g->sounds[0] = LoadSound("sounds/explosion.mp3");
    g->sounds[1] = LoadSound("sounds/click.mp3");
    g->sounds[2] = LoadSound("sounds/bubble.mp3");

    initPlayer(g, &g->players[0], p1_nome, WHITE,
        (Rectangle){1 * STD_SIZE + STD_SIZE_DIF, 1 * STD_SIZE + STD_SIZE_DIF, STD_SIZE_ENT, STD_SIZE_ENT});
    
    initPlayer(g, &g->players[1], p2_nome, BLACK,
        (Rectangle){13 * STD_SIZE + STD_SIZE_DIF, 13 * STD_SIZE + STD_SIZE_DIF, STD_SIZE_ENT, STD_SIZE_ENT});


    mapSetup(g, map);
    g->font = font;
    return g;
}

void freeGame(Game* game) {
    for (int i = 0; i < 2; i++) {
        UnloadTexture(game->players[i].sprite.self);
        UnloadTexture(game->players[i].sprite_bomb);
        UnloadTexture(game->players[i].sprite_explosion);
        for (int j = 0; j < 5; j++) {
            for (int k = 0; k < 2; k++) {
                UnloadSoundAlias(game->players[i].bombs[j].sounds[k]);
            }
        }
    }
    UnloadTexture(game->pickups_sprite);
    UnloadMusicStream(game->music);
    for (int i = 0; i < 3; i++) {
        UnloadSound(game->sounds[i]);
    }
    for (int i = 0; i < 1 + game->map.map_num; i++) {
        UnloadTexture(game->map.sprite[i]);
    }
    free(game->map.sprite);
    free(game->map.especial);
    free(game);
}

void updateGame(Game* game) {
    UpdateMusicStream(game->music);
    game->time = GetTime() - game->start_time;

    if (game->time < 120) {
        updatePlayersPos(game);
    } else {
        for (int i = 0; i < 5; i++) {
            game->players[0].bombs[i].fastExplode = 1;
            game->players[1].bombs[i].fastExplode = 1;
        }
        int cabou = 1;
        for (int i = 0; i < 5; i ++) {
            if (game->players[0].bombs[i].isActive || game->players[1].bombs[i].isActive) {
                cabou = 0;
            }
        }
        if (cabou) {
            game->players[0].vivo = 0;
            game->players[1].vivo = 0;
        }
    }

    switch (game->map.map_num) {
        case 0:
            break;
        case 1:
            if (GetTime() - game->map.pucci_steal_time > 20 &&
                game->map.pucci_pickup_steal_info[3] == 1) {
                int p_indice = game->map.pucci_pickup_steal_info[4];
                game->players[p_indice].speed += game->map.pucci_pickup_steal_info[0];
                game->players[p_indice].num_bombs += game->map.pucci_pickup_steal_info[1];
                game->players[p_indice].bomb_distance += game->map.pucci_pickup_steal_info[2];
                game->map.pucci_pickup_steal_info[3] = 0;
            } 
            if (!game->map.pucci_pickup_steal_info[3]) {
                updatePucci(game);
                colPucciPlayer(game, &game->players[0], 0);
            }
            if (!game->map.pucci_pickup_steal_info[3]) colPucciPlayer(game, &game->players[1], 1);
            break;
    }
    colPlayerPickups(game, &game->players[0]);
    colPlayerPickups(game, &game->players[1]);
    updateBombs(game);
    if (colExplosion(game->players[0].bombs, game->players[0].num_bombs, game->players[0].pos) ||
        colExplosion(game->players[1].bombs, game->players[1].num_bombs, game->players[0].pos)) {
        game->players[0].vivo = 0;
    }
    if (colExplosion(game->players[0].bombs, game->players[0].num_bombs, game->players[1].pos) ||
        colExplosion(game->players[1].bombs, game->players[1].num_bombs, game->players[1].pos)) {
        game->players[1].vivo = 0;
    }
}

void DrawGame(Game *game, Placar* placar) {
    BeginDrawing();
    ClearBackground((Color){181, 85, 21, 255});

    draw_map(&game->map);
    drawPickup(&game->pickups_sprite, game->pickups, game->total_pickups);

    draw_bomb(&game->players[0]);
    draw_bomb(&game->players[1]);

    drawPlayerSprite(game, &game->players[0]);
    drawPlayerSprite(game, &game->players[1]);

    DrawText(game->players[0].nome, 640, 10, 30, game->players[0].color);
    DrawText(game->players[1].nome, 630, 40, 30, game->players[1].color);

    drawEspecials(game);

    if (game->time < 120) {
        DrawText(
            TextFormat("%02d:%02d:%02d\n", 1 - (int)(game->time/60),
                                          59 - (int) game->time % 60,
                                          99 - (int)(game->time * 100) % 100),
            630, 100, 40, BLACK);
    } else {
        DrawText("TIMES UP!", 610, 100, 34, BLACK);
    }
    
    EndDrawing();
}

void gameLoop(Game* game, Placar* placar) {
    updateGame(game);
    DrawGame(game, placar);
}
