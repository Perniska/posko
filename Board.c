// Board.c
#include "Board.h"
#include <stdlib.h>

Board createBoard(int width, int height) {
    Board board;
    int i, j;

    board.width = width;
    board.height = height;

    // Allocate memory for the grid
    board.grid = (char**)malloc(height * sizeof(char*));
    for (i = 0; i < height; ++i) {
        board.grid[i] = (char*)malloc(width * sizeof(char));
    }

    // Initialize the grid with '.'
    for (i = 0; i < height; ++i) {
        for (j = 0; j < width; ++j) {
            board.grid[i][j] = '.';
        }
    }

    return board;
}

void drawBoard(const Board* board) {
    int i, j;
    for (i = 0; i < board->height; ++i) {
        for (j = 0; j < board->width; ++j) {
            printf("%c ", board->grid[i][j]);
        }
        printf("\n");
    }
}

void clearBoard(Board* board) {
    int i, j;
    for (i = 0; i < board->height; ++i) {
        for (j = 0; j < board->width; ++j) {
            board->grid[i][j] = '.';
        }
    }
}

void updateSnakePosition(Board* board, const Position* snakeBody, int snakeLength) {
    int i, x, y;

    // Clear the matrix for the new snake position
    clearBoard(board);

    // Update the matrix with the new snake position
    for (i = 0; i < snakeLength; ++i) {
        x = getPositionX(&snakeBody[i]) % board->width;
        y = getPositionY(&snakeBody[i]) % board->height;
        board->grid[y][x] = 'S';
    }
}

void updateFoodPosition(Board* board, const Position* foodPosition) {
    // Update the position of the food in the matrix
    int x = getPositionX(foodPosition) % board->width;
    int y = getPositionY(foodPosition) % board->height;
    board->grid[y][x] = 'F';
}

int getBoardWidth(const Board* board) {
    return board->width;
}

int getBoardHeight(const Board* board) {
    return board->height;
}

void destroyBoard(Board* board) {
    // Uvoľnenie pamäte pre každý riadok matice
    for (int i = 0; i < board->height; ++i) {
        free(board->grid[i]);
    }

    // Uvoľnenie pamäte pre pole riadkov
    free(board->grid);

    // Nastavenie veľkostí na 0, aby sme mohli ľahko identifikovať uvoľnenú dosku
    board->width = 0;
    board->height = 0;
}
