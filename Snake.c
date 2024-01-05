// Snake.c
#include "Snake.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

//vytvorenie hada
Snake createSnake() {
    Snake snake;
    snake.direction = RIGHT;
    snake.body = NULL;
    snake.length = 0;
    return snake;
}

// Vytvorenie hada s konkrétnou počiatočnou pozíciou
Snake createSnakeWithPosition(int initialX, int initialY) {
    // Vytvoríme hada pomocou základnej funkcie createSnake(), ktorá inicializuje základné hodnoty.
    Snake snake = createSnake();

    // Alokovanie pamäti pre pole pozícií hada (prvý prvok, inicializačný bod)
    snake.body = malloc(sizeof(Position));

    // Kontrola chyby alokácie pamäti
    if (snake.body == NULL) {
        //v prípade chyby alokácie, sa vyhodií chybová správs
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    // Nastavenie počiatočnej pozície hada
    snake.body[0].x = initialX;
    snake.body[0].y = initialY;

    // Nastavenie dĺžky hada na 1 (iba jedna počiatočná pozícia)
    snake.length = 1;

    // Nastavenie počiatočnej pozície hlavy hada na počiatočnú pozíciu
    snake.head = snake.body[0];

    // Vrátenie vytvoreného hada
    return snake;
}


void moveSnake(Snake* snake, int width, int height) {
    // Posun všetkých členov hada smerom k hlave
    for (int i = snake->length - 1; i > 0; --i) {
        snake->body[i] = snake->body[i - 1];
    }
    // Aktualizujte pozíciu hlavy hada na základe smeru pohybu a rieši wrap-around
    switch (snake->direction) {
        case LEFT:
            //Posun vyjadruje -1, +width zabezbečuje kladne číslo aby sme nevypadli z hracej plochy
            //Vykoná operáciu modulo so šírkou hracej plochy. To zabezpečuje, že hodnota sa obalí (wrap) späť na začiatok, keď prejde cez pravú hranicu.
            snake->body[0].x = (snake->body[0].x - 1 + width) % width;
            break;
        case DOWN:
            snake->body[0].y = (snake->body[0].y + 1) % height;
            break;
        case RIGHT:
            snake->body[0].x = (snake->body[0].x + 1) % width;
            break;
        case UP:
            snake->body[0].y = (snake->body[0].y - 1 + height) % height;
            break;
    }
}

void growSnake(Snake* snake) {
    // Zvýšenia dĺžky hada
    snake->length++;

    // Reallokujte pamäť pre telo hada
    snake->body = realloc(snake->body, snake->length * sizeof(Position));
    if (snake->body == NULL) {
        // Handle memory allocation error
        exit(EXIT_FAILURE);
    }

    // Vypočet indexu novej pozície
    int newIndex = snake->length - 1;

    //Kopírovanie pozície na novú poziciu
    memcpy(&(snake->body[newIndex]), &(snake->body[newIndex - 1]), sizeof(Position));

    // Inkrementuje súradnicu na základe smeru hada
    switch (snake->direction) {
        case LEFT:
            snake->body[newIndex].x += 1;
            break;
        case DOWN:
            snake->body[newIndex].y -= 1;
            break;
        case RIGHT:
            snake->body[newIndex].x -= 1;
            break;
        case UP:
            snake->body[newIndex].y += 1;
            break;
    }
}



int checkCollision(const Snake* snake) {
    //Kontroluje ci hlava hada narazila do tela hada
    for (int i = 1; i < snake->length; ++i) {
        if (snake->body[0].x == snake->body[i].x && snake->body[0].y == snake->body[i].y) {
            printf("Snake collided with itself!\n");
            return 1; // True, kolízia
        }
    }
    return 0; // False, žiadna kolízia
}

void updateDirection(Snake* snake, char key) {
    //nastavenie smeru hada na základe vstupu od používateľa
    key = tolower(key);
    switch(key) {
        case 'a' :
            snake->direction = LEFT;
            break;
        case 's' :
            snake->direction = DOWN;
            break;
        case 'd' :
            snake->direction = RIGHT;
            break;
        case 'w' :
            snake->direction = UP;
            break;
    }
}

//vracia pozíciu hlavy hada
Position getSnakePosition(const Snake* snake) {
    return snake->body[0];
}

Position* getSnakeBody(const Snake* snake) {
    return snake->body;
}

void destroySnake(Snake* snake) {
    free(snake->body);
    snake->body = NULL;
    snake->length = 0;
}
