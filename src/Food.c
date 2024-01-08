// Food.c
#include "Food.h"
#include <stdlib.h>

Food createFood() {
    Food food;
    food.position = createPosition(0, 0);
    return food;
}

Position getFoodPosition(const Food* food) {
    return food->position;
}

void generateNewFoodPosition(Food* food, int maxWidth, int maxHeight) {
    setPositionX(&(food->position), rand() % maxWidth);
    setPositionY(&(food->position), rand() % maxHeight);
}
