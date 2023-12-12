#include "game.h"
#include "bombas.h"
#include "maps.h"
#include "menu.h"

#include "raylib.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "string.h"

Game* initGame(int map, char* p1_nome, char* p2_nome, Font* font){
    Game* g = (Game*)malloc(sizeof(Game));
    g->pickups_sprite = LoadTexture("sprites/pickups.png");
    g->total_pickups = 0;
    g->start_time = GetTime() + 3.2;
    g->time = g->start_time - GetTime();
    g->end_time = 0;
    g->end_game = 0;
    g->exit_game = 0;

    if (map == 0) {
        g->music = LoadMusicStream("sounds/Undertale OST - Another Medium.mp3");
        SeekMusicStream(g->music, 105);
        SetMusicVolume(g->music, 0.6);
    } else {
        g->music = LoadMusicStream("sounds/cathedral.mp3");
        SetMusicVolume(g->music, 0.8);
    }
    PlayMusicStream(g->music);

    g->sounds[0] = LoadSound("sounds/explosion.mp3");
    g->sounds[1] = LoadSound("sounds/click.mp3");
    g->sounds[2] = LoadSound("sounds/bubble.mp3");
    g->sounds[3] = LoadSound("sounds/countdown.mp3");

    initPlayer(g, &g->players[0], p1_nome,
        (Rectangle){1 * STD_SIZE + 5, 1 * STD_SIZE + STD_SIZE_DIF, 30, STD_SIZE_ENT},
        (Color){128, 228, 228, 255});
    
    initPlayer(g, &g->players[1], p2_nome,
        (Rectangle){13 * STD_SIZE + 5, 13 * STD_SIZE + STD_SIZE_DIF, 30, STD_SIZE_ENT},
        (Color){206, 161, 154, 255});

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
    for (int i = 0; i < 4; i++) {
        UnloadSound(game->sounds[i]);
    }
    for (int i = 0; i < 1 + 2 * game->map.map_num; i++) { // map 0: 0, map 1: 3
        UnloadTexture(game->map.sprite[i]);
    }
    free(game->map.sprite);
    free(game->map.especial);
    free(game);
}

void endGame(Game* game) {
    if (game->end_game == 0) {
        game->end_time = game->time + 3;
        game->end_game = 1;
        StopMusicStream(game->music);
    }
    if (game->time - game->end_time > 0) {
        game->exit_game = 1;
    }
}

void timeOut(Game* game) {
    endGame(game);
    for (int i = 0; i < 5; i++) {
        game->players[0].bombs[i].fastExplode = 1;
        game->players[1].bombs[i].fastExplode = 1;
    }
    for (int i = 0; i < 5; i ++) {
        if (game->players[0].bombs[i].isActive || game->players[1].bombs[i].isActive) {
            break;
        }
        game->players[0].vivo = 0;
        game->players[1].vivo = 0;
    }
}

void updateGame(Game* game) {
    UpdateMusicStream(game->music);
    game->time = GetTime() - game->start_time;
    if (game->time < 0) return;
    if (game->time > 120) timeOut(game);

    if (game->end_game == 0) {
        updatePlayersPos(game);
        if (game->map.map_num == 1 && game->time > 45) {
            updateDelirium(game);
            if (game->map.delirium_pickup_steal_info[3] == 0) {
                colDeliriumPlayer(game, &game->players[0], 0);
                colDeliriumPlayer(game, &game->players[1], 1);
            }
        }

        colPlayerPickups(game, &game->players[0]);
        colPlayerPickups(game, &game->players[1]);
        if (colExplosion(game->players[0].bombs, game->players[0].num_bombs, game->players[0].pos) ||
            colExplosion(game->players[1].bombs, game->players[1].num_bombs, game->players[0].pos)) {
            game->players[0].vivo = 0;
            endGame(game);
        }
        if (colExplosion(game->players[0].bombs, game->players[0].num_bombs, game->players[1].pos) ||
            colExplosion(game->players[1].bombs, game->players[1].num_bombs, game->players[1].pos)) {
            game->players[1].vivo = 0;
            endGame(game);
        }
    } else {
        endGame(game);
    }
    updateBombs(game);
}

void drawPlayerInfo(Game* game, Player* player, Vector2 pos) {
    int font_size = 40;
    while (MeasureTextEx(*game->font, player->nome, font_size, font_size/10).x > 160) {
        font_size--;
    }
    DrawTextEx(*game->font, player->nome, (Vector2){pos.x + 10, pos.y}, font_size, font_size/10, player->color);

    DrawTexturePro(game->pickups_sprite, (Rectangle){0, 0, 320, 320}, (Rectangle){pos.x + 30, pos.y + 65, 60, 60},
    (Vector2){20, 20}, 0, WHITE);
    DrawTextEx(*game->font, TextFormat(" %d", player->speed), (Vector2){pos.x + 80, pos.y + 55}, 40, 3, WHITE);


    DrawTexturePro(game->pickups_sprite, (Rectangle){420, 80, 120, 160}, (Rectangle){pos.x + 30, pos.y + 115, 40, 40},
    (Vector2){10, 18}, 0, WHITE);
    DrawTextEx(*game->font, TextFormat(" %d", player->bomb_distance), (Vector2){pos.x + 80, pos.y + 105}, 40, 3, WHITE);


    DrawTexturePro(game->pickups_sprite, (Rectangle){634, 0, 320, 180}, (Rectangle){pos.x + 30, pos.y + 145, 40, 40},
    (Vector2){10, 5}, 0, WHITE);
    DrawTextEx(*game->font, TextFormat(" %d", player->num_bombs), (Vector2){pos.x + 80, pos.y + 145}, 40, 3, WHITE);
}

void drawSideMenu(Game* game) {
    DrawRectangle(600, 0, 200, 600, (Color){24, 24, 24, 255});
    DrawRectangle(600, 0, 2, 600, GOLD);

    if (game->time < 0) {
        Vector2 size = MeasureTextEx(*game->font, "02:00", 40, 4);
        DrawTextEx(*game->font, "02:00", (Vector2){700 - size.x/2, 100 - size.y/2}, 40, 4, WHITE);
    } else if (game->time < 120) {
        char timer[20];
        strcpy(timer, TextFormat("%02d:%02d", 1 - (int)(game->time/60), 59 - (int) game->time % 60));
        Vector2 size = MeasureTextEx(*game->font, timer, 40, 4); 
        DrawTextEx(*game->font, timer,
            (Vector2){700 - size.x/2, 100 - size.y/2}, 40, 4, WHITE);
    } else {
        DrawTextEx(*game->font, "TIMES UP!", (Vector2){600, 50}, 34, 4, WHITE);
    }   

    drawPlayerInfo(game, &game->players[0], (Vector2){610, 150});
    drawPlayerInfo(game, &game->players[1], (Vector2){610, 350});
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

    drawEspecials(game);

    drawSideMenu(game);

    if (game->time < 1) {
        char text[10];
        if ((int)game->time == 0 && game->time > 0) {
            strcpy(text, "IT'S ON!");
        } else {
            strcpy(text, TextFormat("%d...", -(int)game->time + 1));
            if (!IsSoundPlaying(game->sounds[3])) {
                PlaySound(game->sounds[3]);
            }
        }
        drawTextBox(game->font, text, (Vector2){300, 300}, 100, 10, WHITE);
    }
    EndDrawing();
}

void gameLoop(Game* game, Placar* placar) {
    DrawGame(game, placar);
    updateGame(game);
}
