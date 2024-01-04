// Position.h
#ifndef POSITION_H
#define POSITION_H

#include <stdio.h>

typedef struct {
    int x;
    int y;
} Position;

Position createPosition(int x, int y);
int getPositionX(const Position* position);
int getPositionY(const Position* position);
void setPositionX(Position* position, int newX);
void setPositionY(Position* position, int newY);
int positionEquals(const Position* position1, const Position* position2);

#endif // POSITION_H
