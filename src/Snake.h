// Snake.h
#ifndef SNAKE_H
#define SNAKE_H

#include "Position.h"
#include "Direction.h"
#include <string.h>



typedef struct {
    Direction direction;
    Position* body; // Pole pre uchovávanie pozícií tela hada
    int length; // Dĺžka hada
    Position head;
} Snake;

Snake createSnake();
Snake createSnakeWithPosition(int initialX, int initialY);
void moveSnake(Snake* snake, int width, int height);
void growSnake(Snake* snake);
int checkCollision(const Snake* snake);
void updateDirection(Snake* snake, char key);
Position getSnakePosition(const Snake* snake);
Position* getSnakeBody(const Snake* snake);
void destroySnake(Snake* snake);

#endif // SNAKE_H
