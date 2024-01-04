// Board.h
#ifndef BOARD_H
#define BOARD_H

#include "Position.h"
#include <stdio.h>

typedef struct {
    int width;
    int height;
    char** grid;
} Board;

Board createBoard(int width, int height);
void drawBoard(const Board* board);
void clearBoard(Board* board);
void updateSnakePosition(Board* board, const Position* snakeBody, int snakeLength);
void updateFoodPosition(Board* board, const Position* foodPosition);
int getBoardWidth(const Board* board);
int getBoardHeight(const Board* board);

#endif // BOARD_H
