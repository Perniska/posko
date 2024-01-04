#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "hra.h"
#include "Game.h"  // Predpokladám, že ste vytvorili vlastný súbor Game.h pre vašu hru

int hra() {
    srand((unsigned)time(NULL)); // Seed pre generátor náhodných čísel

    // Vytvorenie inštancie hry s určitou šírkou a výškou
    Game game;
    initializeGame(&game, 10, 10);  // Predpokladám, že ste vytvorili vlastnú funkciu initializeGame

    // Spustenie hry
    runGame(&game);  // Predpokladám, že ste vytvorili vlastnú funkciu runGame

    return 0;
}
