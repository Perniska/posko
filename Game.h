// Game.h
#ifndef GAME_H
#define GAME_H

#include "Board.h"
#include "Snake.h"
#include "Food.h"

typedef struct {
    Board board;
    Snake snake;
    Food food;
} Game;

void initializeGame(Game* game, int width, int height);
void runGame(Game* game);

#endif // GAME_H
