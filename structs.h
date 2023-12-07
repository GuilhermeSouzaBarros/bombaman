#include "raylib.h"

#ifndef STRUCTS
#define STRUCTS

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define STD_SIZE 40
#define STD_SIZE_ENT 36
#define STD_SIZE_DIF 2

typedef struct Barriers {
    Rectangle barriers[15][15];
    int types[15][15]; //| 0: Nothing | 1: Wall | 2: Destructible | 3: Pickup |
    Rectangle sprite_pos[15][15];
} Barriers;

typedef struct Map {
    int map_num;
    Texture2D sprite;
    Barriers barriers;
    Rectangle* especial;
    int n_especiais;
    int pucci_pickup_steal_info[5];
    double pucci_steal_time;
    int num_barriers_line;
    int num_barriers_coln;
    Color color;
} Map;

typedef struct Pickup {
    Rectangle pos;
    int visible;
    int tile_x;
    int tile_y;
    Color color;
    int type; //| 0: Speed | 1: Num_bombs | 2: Range |
} Pickup;

typedef struct Bomb{
    Rectangle pos;
    Rectangle explosions[4]; // | 0: up | 1: left | 2: down | 3: right |
    int stop_explosion[4];   // | i0: up | i1: left | i2: down | i3: right | 1: stop;
    int isActive;
    int isActiveFirstFrame;
    int hasExploded;
    int distance;
    double time;
    int hasColision;
    int fastExplode;
}Bomb;

typedef struct Player {
    Rectangle pos;
    Color color;
    Texture2D sprite;
    Texture2D sprite_bomb;
    Texture2D sprite_explosion;
    int facing; // | 0: down | 1: left | 2: right | 3: up |
    int is_moving;
    char* nome;
    int speed;
    Bomb bombs[5];
    int bomb_distance;
    int num_bombs;
    int vivo;
} Player;

typedef struct Game {
    Map map;
    Player players[2];
    Pickup pickups[10];
    int total_pickups;
    double time;
    double start_time;
} Game;

typedef struct Menu {
    int screen;
    int game_start;
    int text_input_1;
    int text_input_2;
    char* p1_nome;
    int p1_qletras;
    char* p2_nome;
    int p2_qletras;
    int map;
} Menu;

typedef struct EndMenu {
    char p1_nome[10];
    char p2_nome[10];
    int winner; // | 0: Draw |
    Color winner_color;
    int exit;
    int num_map;
} EndMenu;

typedef struct Placar {
    int p1_wins;
    int p2_wins;
    int draws;
    char p1_nome[10];
    char p2_nome[10];
    int rematch;
    int next_map;
} Placar;

#endif