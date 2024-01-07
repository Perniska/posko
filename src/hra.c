#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "hra.h"
#include "Game.h"

int hra() {
    srand((unsigned)time(NULL)); // Seed pre generátor náhodných čísel

    // Vytvorenie inštancie hry s určitou šírkou a výškou
    Game game;
    initializeGame(&game, 10, 10);

    // Spustenie hry
    runGame(&game);

    return 0;
}
