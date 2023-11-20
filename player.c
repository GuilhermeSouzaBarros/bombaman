#include "player.h"

void update_player_pos(Player *h, Map *m){
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