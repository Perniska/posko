#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "Game.h"

int generateUniqueID() {
    // Using a combination of timestamp and random number for a more unique ID
    int id = (int)(time(NULL) * 100) + rand() ;
    return id;
}

int waitForSecondPlayer(int sockfd) {
    char buffer[256];
    int n;

    // Wait for the signal from the server indicating the second player has connected
    while (1) {
        bzero(buffer, sizeof(buffer));
        n = read(sockfd, buffer, sizeof(buffer) - 1);
        if (n < 0) {
            perror("Error reading from socket");
            return 1; // Handle error
        }

        // Check for the signal
        if (strcmp(buffer, "START_GAME") == 0) {
            printf("Both players connected. Starting the game!\n");
            break;
        }

        // Optionally, you can implement a timeout mechanism here

        // Sleep for a short interval to avoid busy-waiting
        usleep(100000); // 100 milliseconds
    }

    return 0; // Success
}

int main(int argc, char *argv[]) {
    int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent *server; // informacie o serveri
    char buffer[256];

    // pocet argumentov
    if (argc < 3) {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        return 1;
    }

    // informacie o serveri na ktory sa idem pripajat ziskam pomocou funkcie gethostbyname
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr, "Error, no such host\n");
        return 2;
    }

    // informacie o serveri na ktory sa pripajam
    bzero((char *)&serv_addr, sizeof(serv_addr));
    // internetove sockety
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);

    // port
    serv_addr.sin_port = htons(atoi(argv[2]));
    // vytvoenie socketu
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error creating socket");
        return 3;
    }

    // cez ktory sockety sa kam pripajam, cakam na odpoved servera
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Error connecting to socket");
        return 4;
    }

    printf("Client: Connection established! Hello, server! 🤝\n");

    // kod programu
    char *user = getenv("USER");
    bzero(buffer, 256);
    int id = generateUniqueID();
    // Combine the username and unique ID into a single string with the expected format
    snprintf(buffer, sizeof(buffer), "%s, ID: %d", user, id);
    n = write(sockfd, buffer, strlen(buffer));
    if (n < 0) {
        perror("Error writing username and ID to socket");
        return 5;
    }
      // Wait for the second player to connect
    if (waitForSecondPlayer(sockfd) != 0) {
        // Handle error
        return 7;
    }

    printf("Client: Welcome, %s! The game is starting now.\n", user);


    srand((unsigned)time(NULL)); // Seed pre generátor náhodných čísel

    Game game;
    initializeGame(&game, 10, 10);  // Predpokladám, že ste vytvorili vlastnú funkciu initializeGame

    // Spustenie hry
    runGame(&game);  // Predpokladám, že ste vytvorili vlastnú funkciu runGame

    bzero(buffer, 256);

    // Combine the username and unique ID into a single string with the expected format
    snprintf(buffer, sizeof(buffer), "%s, ID: %d  , score: %d", user, id ,getScore(&game));
    n = write(sockfd, buffer, strlen(buffer));
    if (n < 0) {
        perror("Error writing username ,ID and score to socket");
        return 5;
    }


    // Continue with the rest of your game logic...

    // uzavrenie socketu
    close(sockfd);

    return 0;
}