// Food.h
#ifndef FOOD_H
#define FOOD_H

#include "Position.h"

typedef struct {
    Position position;
} Food;

Food createFood();
Position getFoodPosition(const Food* food);
void generateNewFoodPosition(Food* food, int maxWidth, int maxHeight);

#endif // FOOD_H
