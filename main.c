#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

//----------------------------------------------------------------------------------
// Some Defines
//----------------------------------------------------------------------------------
#define STD_SIZE_X 40
#define STD_SIZE_Y 40
#define STD_SIZE_ENT_X 32
#define STD_SIZE_ENT_Y 32
#define STD_SIZE_DIF_X 4
#define STD_SIZE_DIF_Y 4

#define SCREEN_BORDER 4

typedef struct Bomb{
    Rectangle pos;
    Rectangle explosion_right;
    Rectangle explosion_left;
    Rectangle explosion_up;
    Rectangle explosion_down;
    int growth_ratio;
    int isActive;
    int isActiveFirstFrame;
    int distance;
    int time;
}Bomb;

typedef struct Hero {
    Rectangle pos;
    Color color;
    int speed;
    int special;
    int draw_bomb;
    int put_bomb;
    int num_bombs;
    Bomb bombs[10];
} Hero;

typedef struct Map {
    Rectangle barriers[15][15];
    int num_barriers_line;
    int num_barriers_coln;
    Color color;
    int next_map;
    int prev_map;
} Map;

typedef struct Game { 
    Map maps[10];
    int num_maps;
    int curr_map;
    Hero hero;
    int screenWidth;
    int screenHeight;
    int gameover;
} Game;

//------------------------------------------------------------------------------------
// Module Functions Declaration (local)
//------------------------------------------------------------------------------------
void InitGame(Game *g);         // Initialize game
void UpdateGame(Game *g);       // Update game (one frame)
void DrawGame(Game *g);         // Draw game (one frame)
void UpdateDrawFrame(Game *g);  // Update and Draw (one frame)

//------------------------------------------------------------------------------------
// Auxiliar Functions Declaration
//------------------------------------------------------------------------------------
void draw_map(Game *g);
void draw_bomb(Game *g);
void update_hero_pos(Game *g);
void update_bomb(Game *g);

int barrier_collision(Map *m, Rectangle t);
void map0_setup(Game *g);

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    Game game;
    game.screenWidth = 800;
    game.screenHeight = 600;

    InitWindow(game.screenWidth, game.screenHeight, "BOMBAMAN");
    SetTargetFPS(60);
    
    InitGame(&game);

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        UpdateDrawFrame(&game);
        if(game.gameover) break;
    }
    while(!IsKeyDown(KEY_ENTER)){
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("GAME OVER", GetScreenWidth()/2 - MeasureText("GAME OVER", 20)/2, GetScreenHeight()/2 - 50, 20, BLACK);
        EndDrawing();
    }
    return 0;
}
//------------------------------------------------------------------------------------
// Module Functions Definitions (local)
//------------------------------------------------------------------------------------

// Initialize game variables
void InitGame(Game *g){

    g->curr_map = 0;
    g->num_maps = 10;
    g->hero.pos = (Rectangle) { 40, 40, STD_SIZE_ENT_X, STD_SIZE_ENT_Y};
    g->hero.color = ORANGE;
    g->hero.speed = 4;
    g->hero.special = 0;
    g->gameover = 0;
    g->hero.num_bombs = 5;
    g->hero.put_bomb = 0;
    g->hero.draw_bomb = 0;
    map0_setup(g);
}

// Update game (one frame)
void UpdateGame(Game *g)
{
    update_hero_pos(g);
    update_bomb(g);

    Map *map = &g->maps[g->curr_map];
}

// Draw game (one frame)
void DrawGame(Game *g)
{
    BeginDrawing();

    ClearBackground(RAYWHITE);
    DrawRectangle(0, 0, g->screenWidth, g->screenHeight, DARKGRAY);
    draw_map(g);
    draw_bomb(g);
    DrawRectangleRec(g->hero.pos, g->hero.color);

    EndDrawing();
}

// Update and Draw (one frame)
void UpdateDrawFrame(Game *g)
{
    UpdateGame(g);
    DrawGame(g);
}

void draw_map(Game *g){
    Map *map = &g->maps[g->curr_map];
    for(int i = 0; i < map->num_barriers_line; i++){
        for (int j = 0; j < map->num_barriers_coln; j++) {
            DrawRectangleRec(map->barriers[i][j], GRAY);
        }
    }
}

void draw_bomb(Game *g){
    for(int i = 0; i < g->hero.num_bombs; i++){
        if(g->hero.bombs[i].isActive == 1){
            DrawRectangleRec(g->hero.bombs[i].explosion_right, RED);
            DrawRectangleRec(g->hero.bombs[i].explosion_left, RED);
            DrawRectangleRec(g->hero.bombs[i].explosion_up, RED);
            DrawRectangleRec(g->hero.bombs[i].explosion_down, RED);
            if (g->hero.bombs[i].isActiveFirstFrame) {
                DrawRectangleRec(g->hero.bombs[i].pos, BLACK);
            }
        }
    }
}

void update_hero_pos(Game *g){
    Hero *h = &g->hero;
    Map *m = &g->maps[g->curr_map];
    int vel_x = 0, vel_y = 0;
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) vel_x += h->speed;
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) vel_x -= h->speed;
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) vel_y -= h->speed;
    if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) vel_y += h->speed;
    do {
        int col=0;
        h->pos.x += vel_x;
        for (int i = 0; i < m->num_barriers_line; i++) {
            for (int j = 0; j < m->num_barriers_coln; j++) {
                if (CheckCollisionRecs(m->barriers[i][j], h->pos)) {
                    h->pos.x -= vel_x;
                    if (vel_x > 0) { 
                        vel_x--;
                    } else {
                        vel_x++;
                    }
                    col = 1;
                    break;
                }
            }
        }
        if (col) {
            col = 0;
            continue;
        }
        vel_x = 0;
    } while (vel_x);
    do {
        int col=0;
        h->pos.y += vel_y;
        for (int i = 0; i < m->num_barriers_line; i++) {
            for (int j = 0; j < m->num_barriers_coln; j++) {
                if (CheckCollisionRecs(m->barriers[i][j], h->pos)) {
                    h->pos.y -= vel_y;
                    if (vel_y > 0) vel_y--; else vel_y++;
                    col = 1;
                    break;
                }
            }
        }
        if (col) {
            col = 0;
            continue;
        }
        vel_y = 0;
    } while (vel_y);
}

void update_bomb(Game *g){
    Map *actual_map = &g->maps[g->curr_map];

    if(IsKeyPressed(KEY_SPACE)){
        g->hero.put_bomb = 1;
    }

    if(g->hero.put_bomb == 1){
        int hero_tile_x = ((int)g->hero.pos.x + STD_SIZE_ENT_X / 2);
        hero_tile_x -= hero_tile_x % 40 - 4;
        int hero_tile_y = ((int)g->hero.pos.y + STD_SIZE_ENT_Y / 2);
        hero_tile_y -= hero_tile_y % 40 - 4;
        for(int i = 0; i < g->hero.num_bombs; i++){
            if(g->hero.bombs[i].isActive == 0){
                g->hero.bombs[i].isActive = 1;
                g->hero.bombs[i].isActiveFirstFrame = 1;
                g->hero.bombs[i].growth_ratio = 2;
                g->hero.bombs[i].pos = (Rectangle) {hero_tile_x, hero_tile_y, STD_SIZE_ENT_X, STD_SIZE_ENT_Y};
                g->hero.bombs[i].explosion_right = (Rectangle) {hero_tile_x, hero_tile_y, STD_SIZE_ENT_X, STD_SIZE_ENT_Y};
                g->hero.bombs[i].explosion_left = (Rectangle) {hero_tile_x, hero_tile_y, STD_SIZE_ENT_X, STD_SIZE_ENT_Y};
                g->hero.bombs[i].explosion_down = (Rectangle) {hero_tile_x, hero_tile_y, STD_SIZE_ENT_X, STD_SIZE_ENT_Y};
                g->hero.bombs[i].explosion_up = (Rectangle) {hero_tile_x, hero_tile_y, STD_SIZE_ENT_X, STD_SIZE_ENT_Y};

                g->hero.bombs[i].time = GetTime();
                break;
            }
        }
    }
    for(int i = 0; i < g->hero.num_bombs; i++){
        if(g->hero.bombs[i].isActive){
            if(fabs(g->hero.bombs[i].time - GetTime()) > 3 && fabs(g->hero.bombs[i].time - GetTime()) < 5){
                if (g->hero.bombs[i].isActiveFirstFrame) {
                    g->hero.bombs[i].explosion_right = (Rectangle){g->hero.bombs[i].pos.x - STD_SIZE_DIF_X, g->hero.bombs[i].pos.y - STD_SIZE_DIF_Y, STD_SIZE_X, STD_SIZE_Y};
                    g->hero.bombs[i].explosion_left = (Rectangle){g->hero.bombs[i].pos.x - STD_SIZE_DIF_X, g->hero.bombs[i].pos.y - STD_SIZE_DIF_Y, STD_SIZE_X, STD_SIZE_Y};
                    g->hero.bombs[i].explosion_up = (Rectangle){g->hero.bombs[i].pos.x - STD_SIZE_DIF_X, g->hero.bombs[i].pos.y - STD_SIZE_DIF_Y, STD_SIZE_X, STD_SIZE_Y};
                    g->hero.bombs[i].explosion_down = (Rectangle){g->hero.bombs[i].pos.x - STD_SIZE_DIF_X, g->hero.bombs[i].pos.y - STD_SIZE_DIF_Y, STD_SIZE_X, STD_SIZE_Y};
                    g->hero.bombs[i].isActiveFirstFrame = 0;
                }
                Rectangle verify_bomb;
                Rectangle rectangle_bomb;
                if (g->hero.bombs[i].explosion_right.width < g->hero.bombs[i].distance) {
                    rectangle_bomb = g->hero.bombs[i].explosion_right;
                    Rectangle verify_bomb = (Rectangle){ rectangle_bomb.x, rectangle_bomb.y, rectangle_bomb.width + g->hero.bombs[i].growth_ratio - 2,  rectangle_bomb.height};
                    if(!barrier_collision(actual_map, verify_bomb)) {
                        if (verify_bomb.width < g->hero.bombs[i].distance) {
                            g->hero.bombs[i].explosion_right.width += g->hero.bombs[i].growth_ratio;
                        } else {
                            g->hero.bombs[i].explosion_right.width = g->hero.bombs[i].distance;
                        }
                    } else {
                        g->hero.bombs[i].explosion_right.width -= (int)g->hero.bombs[i].explosion_right.width % 40 + 1;
                    }
                }
                if (g->hero.bombs[i].explosion_left.width < g->hero.bombs[i].distance) {
                    rectangle_bomb = g->hero.bombs[i].explosion_left;
                    Rectangle verify_bomb = (Rectangle){ rectangle_bomb.x - g->hero.bombs[i].growth_ratio, rectangle_bomb.y, rectangle_bomb.width + g->hero.bombs[i].growth_ratio,  rectangle_bomb.height};
                    if(!barrier_collision(actual_map, verify_bomb)){
                        if (verify_bomb.width < g->hero.bombs[i].distance) {
                            g->hero.bombs[i].explosion_left.x -= g->hero.bombs[i].growth_ratio;
                            g->hero.bombs[i].explosion_left.width += g->hero.bombs[i].growth_ratio;
                        } else {
                            g->hero.bombs[i].explosion_left.x -= g->hero.bombs[i].distance - g->hero.bombs[i].explosion_left.width;
                            g->hero.bombs[i].explosion_left.width = g->hero.bombs[i].distance;
                        }
                    } else {
                        g->hero.bombs[i].explosion_left.x -= (int)g->hero.bombs[i].explosion_left.x % 40;
                        g->hero.bombs[i].explosion_left.width -= (int)g->hero.bombs[i].explosion_left.width % 40;
                    }
                }
                if (g->hero.bombs[i].explosion_up.height < g->hero.bombs[i].distance) {
                    rectangle_bomb = g->hero.bombs[i].explosion_up;
                    Rectangle verify_bomb = (Rectangle){ rectangle_bomb.x, rectangle_bomb.y - g->hero.bombs[i].growth_ratio, rectangle_bomb.width,  rectangle_bomb.height + g->hero.bombs[i].growth_ratio};
                    if(!barrier_collision(actual_map, verify_bomb)){
                        if (verify_bomb.height < g->hero.bombs[i].distance) {
                            g->hero.bombs[i].explosion_up.y -= g->hero.bombs[i].growth_ratio;
                            g->hero.bombs[i].explosion_up.height += g->hero.bombs[i].growth_ratio;
                        } else {
                            g->hero.bombs[i].explosion_up.y -= g->hero.bombs[i].distance - g->hero.bombs[i].explosion_up.height;
                            g->hero.bombs[i].explosion_up.height = g->hero.bombs[i].distance;
                        }
                    } else {
                        g->hero.bombs[i].explosion_up.y -= (int)g->hero.bombs[i].explosion_up.y % 40;
                        g->hero.bombs[i].explosion_up.height -= (int)g->hero.bombs[i].explosion_up.height % 40 ;
                    }
                }
                if (g->hero.bombs[i].explosion_down.height < g->hero.bombs[i].distance) {
                    rectangle_bomb = g->hero.bombs[i].explosion_down;
                    Rectangle verify_bomb = (Rectangle){ rectangle_bomb.x, rectangle_bomb.y, rectangle_bomb.width,  rectangle_bomb.height + g->hero.bombs[i].growth_ratio - 2};
                    if(!barrier_collision(actual_map, verify_bomb)) {
                        if (verify_bomb.height < g->hero.bombs[i].distance) {
                            g->hero.bombs[i].explosion_down.height += g->hero.bombs[i].growth_ratio;
                        } else {
                            g->hero.bombs[i].explosion_down.height = g->hero.bombs[i].distance;
                        }
                    } else {
                        printf("%2.lf\n", g->hero.bombs[i].explosion_down.y);
                        g->hero.bombs[i].explosion_down.height -= (int)g->hero.bombs[i].explosion_down.height % 40;
                        printf("%2.lf\n", g->hero.bombs[i].explosion_down.height);
                    }
                }
                g->hero.bombs[i].growth_ratio += 2;
            }else if(fabs(g->hero.bombs[i].time - GetTime()) > 3){
                g->hero.bombs[i].isActive = 0;
            }
        }
    }

    g->hero.put_bomb = 0;
}


int barrier_collision(Map *map, Rectangle target){
    for(int i = 0; i < map->num_barriers_line; i++){
        for (int j = 0; j < map->num_barriers_coln; j++) {
            if(CheckCollisionRecs(target, map->barriers[i][j])){
                return 1;
            }
        }
    }
    return 0;
}

// Maps Setup
void map0_setup(Game *g){
    g->maps[0].num_barriers_line = 15;
    g->maps[0].num_barriers_coln = 15;
    for (int i = 0; i < g->maps[0].num_barriers_line; i++) {
        for (int j = 0; j < g->maps[0].num_barriers_coln; j++) {
            if ((i == 0 || j == 0 || i == 14 || j == 14) ||
                (i % 2 == 0 && j % 2 == 0)) {
                g->maps[0].barriers[i][j] = (Rectangle) {40*i, 40*j, 40, 40};
            } else {
                g->maps[0].barriers[i][j] = (Rectangle) {-40, -40, 0, 0};
            }
        }
    }
    g->maps[0].color = BLUE;
    g->maps[0].prev_map = -1;
    g->maps[0].next_map = 1;
    for(int i = 0; i < g->hero.num_bombs; i++){
        g->hero.bombs[i].isActive = 0;
        g->hero.bombs[i].distance = 2 * STD_SIZE_X;
    }
}
