// Position.c
#include "Position.h"

Position createPosition(int x, int y) {
    Position position;
    position.x = x;
    position.y = y;
    return position;
}

int getPositionX(const Position* position) {
    return position->x;
}

int getPositionY(const Position* position) {
    return position->y;
}

void setPositionX(Position* position, int newX) {
    position->x = newX;
}

void setPositionY(Position* position, int newY) {
    position->y = newY;
}

