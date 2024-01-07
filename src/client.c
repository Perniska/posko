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
    return rand(); // For simplicity, using a random number as a unique ID



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
    struct hostent* server; //informacie o serveri
    char buffer[256];

    //pocet argumentov
    if (argc < 3) {
        fprintf(stderr,"usage %s hostname port\n", argv[0]);
        return 1;
    }

    //printf("klient :pocet argumentov je spravny\n");
    //informacie o serveri na ktory sa idem pripajat ziskam pomocou funkcie gethostbyname
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr, "Error, no such host\n");
        return 2;
    }

    //printf("klient :ziskal som informacie o serveri\n");

    //informacie o serveri na ktory sa pripajam
    bzero((char*)&serv_addr, sizeof(serv_addr));
    //internetove sockety
    serv_addr.sin_family = AF_INET;
    bcopy((char*)server->h_addr, (char*)&serv_addr.sin_addr.s_addr, server->h_length);

    //printf("klient :ziskal som informacie o serveri 2\n");
    //port
    serv_addr.sin_port = htons(atoi(argv[2]));
    //printf("klient :nastavil som port\n");
    //vytvoenie socketu
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error creating socket");
        return 3;
    }

    //printf("klient :vytvorenie socketu prebehlo uspesne\n");

    //cez ktory sockety sa kam pripajam ,cakam na odpoved servera
    if(connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Error connecting to socket");
        return 4;
    }

    printf("klient :uspesne som sa pripojil na server\n");

    //kod programu
    char* user = getenv("USER");
    bzero(buffer,256);
    n = write(sockfd, user, strlen(user));
    if (n < 0) {
        perror("Error writing username to socket");
        return 5;
    }

    printf("klient : Spustam hru pre pouzivatela : %s\n" , user);

    srand((unsigned)time(NULL)); // Seed pre generátor náhodných čísel

    // Generate a unique identifier for the client
    int uniqueID = generateUniqueID();

    // Code for sending the username and unique ID to the server
    bzero(buffer, 256);
    // Combine the username and unique ID into a single string
    snprintf(buffer, sizeof(buffer), "%s|%d", user, uniqueID);
    n = write(sockfd, buffer, strlen(buffer));
    if (n < 0) {
        perror("Error writing username and ID to socket");
        return 6;
    }

    // Wait for the second player to connect
    if (waitForSecondPlayer(sockfd) != 0) {
        // Handle error
        return 7;
    }

    // Continue with the rest of your game logic...
    // Vytvorenie inštancie hry s určitou šírkou a výškou
    Game game;
    initializeGame(&game, 10, 10);

    // Spustenie hry
    runGame(&game);

    // Získanie skóre po skončení hry
    int score = getScore(&game);

    // Odoslanie skóre na server
    n = write(sockfd, &score, sizeof(score));
    if (n < 0) {
        perror("Error writing score to socket");
        return 8;
    }

    //uzavrenie socketu
    close(sockfd);

    return 0;
}
