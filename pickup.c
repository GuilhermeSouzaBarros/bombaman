#include "pickup.h"

Pickup* initPickup(int x, int y) {
    Pickup* p = (Pickup*)malloc(sizeof(Pickup));
    p->pos = (Rectangle){x + 4, y + 4, STD_SIZE_ENT_X, STD_SIZE_ENT_Y};
    p->type = rand() % 3;
    switch (p->type) {
        case 0:
            p->color = BLUE;
            break;
        case 1:
            p->color = YELLOW;
            break;
        case 2:
            p->color = GREEN;
            break;
    }
    return p;
}

void drawPickup(Pickup** pickups, int n) {
    for (int i = 0; i < n; i++) {
        DrawRectangleRec(pickups[i]->pos, pickups[i]->color);
    }
}
