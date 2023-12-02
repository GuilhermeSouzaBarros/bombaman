#include "raylib.h"

#ifndef STRUCTS
#define STRUCTS

typedef struct Barriers {
    Rectangle barriers[15][15];
    int types[15][15]; // 0: Nothing; 1: Wall; 2: Destructible
} Barriers;

typedef struct Map {
    Barriers barriers;
    int num_barriers_line;
    int num_barriers_coln;
    Color color;
} Map;

typedef struct Pickup {
    Rectangle pos;
    Color color;
    int type;
} Pickup;

typedef struct Bomb{
    Rectangle pos;
    Rectangle explosions[4]; // | 0: up | 1: left | 2: down | 3: right |
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
    char nome[10];
    int speed;
    Bomb bombs[5];
    int bomb_distance;
    int num_bombs;
    int vivo;
} Player;

typedef struct Game {
    Map map;
    Player players[2];
    Pickup* pickups[10];
    int n_pickups;
    double time;
    double start_time;
} Game;

#endif