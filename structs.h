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

typedef struct Sprite {
    Texture2D self;
    int x_dif;
    int y_dif;
    int x_size;
    int y_size;
    Rectangle death_frame;
    Vector2 offset;
} Sprite;

typedef struct Map {
    int map_num;
    Texture2D* sprite;
    Barriers barriers;
    Rectangle* especial;
    int n_especiais;
    int delirium_pickup_steal_info[5]; //| 0: speed | 1: Num_bombs | 2: Range | 3: ifStole | 4: IDPlayerStolen
    double delirium_steal_time;
    int stun_delirium;
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
    int hasColision[2]; // indice == player
    int fastExplode;
    Sound sounds[2]; // 0: Explosion | 1: Click 
}Bomb;

typedef struct Player {
    Rectangle pos;
    Color color;
    Sprite sprite;
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
    double end_time;
    int end_game;
    int exit_game;
    Font* font;
    Texture2D pickups_sprite;
    Music music; 
    Sound sounds[4]; // 0: Explosion | 1: Click | 2: Bubble | 3: timer
} Game;

typedef struct Menu {
    Texture2D background;
    Texture2D map_icons[2];
    Rectangle pos_sprite;
    Sound click;
    Music music;
    Font* font;
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

typedef struct Placar {
    int p1_wins;
    int p2_wins;
    int draws;
    char p1_nome[10];
    char p2_nome[10];
    int rematch;
    int next_map;
} Placar;

typedef struct EndMenu {
    Texture2D winner_sprite;
    Font* font;
    Music music;
    char p1_nome[10];
    char p2_nome[10];
    int winner; // | 0: Draw |
    Color winner_color;
    int exit;
    int num_map;
    Placar placar;
    Sound click;
} EndMenu;

#endif