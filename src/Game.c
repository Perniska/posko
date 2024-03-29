// Game.c sa stara o priebeh hry
#include "Game.h"
#include <stdio.h>

//Inicializácia hry
void initializeGame(Game* game, int width, int height) {
    //vytvorenie hracej plochy
    game->board = createBoard(width, height);
    // vygenerovanie nahodnej polohy jedla
    generateNewFoodPosition(&(game->food), game->board.width, game->board.height);
    // Nastavenie počiatočnej pozície hada na čiatku hracej plochy
    int initialX = 0;
    int initialY = 0;
    game->snake = createSnakeWithPosition(initialX, initialY);
    game->score = 0;

}

//priebeh hry
void runGame(Game* game) {

    //Na začiatku sa nastavia počiatočne polohy pre jedlo a pre hada a plocha sa vykresli
    updateSnakePosition(&(game->board), getSnakeBody(&(game->snake)), game->snake.length);
    updateFoodPosition(&(game->board), &(game->food.position));

    printf("Score: %d\n", game->score);
    drawBoard(&(game->board));

    while (1) {
        // Aktualizujte smer hada na základe vstupu od používateľa
        char key;
        scanf(" %c", &key);
        if(key == 'q' || key == 'Q') {
            printf("Game Over!\n");
            break;
        }
        updateDirection(&(game->snake), key);

        // Posun hada
        moveSnake(&(game->snake), game->board.width, game->board.height);

        // Kontrola kolízie alebo iné podmienky konca hry
        if (checkCollision(&(game->snake))) {
            printf("Game Over!\n");
            break;
        }

        // Kontrola kolízie s jedlom a rast hada, ak je to potrebné
        if (getSnakePosition(&(game->snake)).x == game->food.position.x &&
            getSnakePosition(&(game->snake)).y == game->food.position.y) {
            growSnake(&(game->snake));
            generateNewFoodPosition(&(game->food), game->board.width, game->board.height);
            //v pripade ze sa jedlo vygenerovalo na pozicii hada, vygeneruje sa znova
            if (checkFoodGeneration (&(game->snake), &(game->food))) {
                generateNewFoodPosition(&(game->food), game->board.width, game->board.height);
            }
            game->score++;
        }

        if(game->score == 10) {
            printf("Congratulations, you win");
            break;
        }
        // Vyčistinie hracej plochy (staré pozicie hracich objektov sa vymazu z plochy)
        clearBoard(&(game->board));

        // Aktualizuje sa nová pozícia hada na hracej ploche
        updateSnakePosition(&(game->board), getSnakeBody(&(game->snake)), game->snake.length);

        // Aktualizuje sa pozícia jedla na hracej ploche
        updateFoodPosition(&(game->board), &(game->food.position));

        // Vykresli sa nová hracia plocha
        printf("Score: %d\n", game->score);
        drawBoard(&(game->board));
    }
    destroySnake(&(game->snake));
    destroyBoard(&(game->board));
}

int getScore(Game* game){
    return game->score;
}

