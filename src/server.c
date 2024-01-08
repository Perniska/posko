#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>



typedef struct {
    int newsockfd;
    char username[256];
    int id;
} ARGUMENTY;

int connectedClients = 0;  // Pocitadlo pripojenych klientov
pthread_mutex_t counterMutex = PTHREAD_MUTEX_INITIALIZER;  // Mutex pre pocitadlo

char* getTime() {
    time_t currentTime;
    struct tm *localTime;

    time(&currentTime);
    localTime = localtime(&currentTime);

    // Alokovanie pamate pre timeString
    char *timeString = (char *)malloc(9); // HH:MM:SS\0

    // Formatovanie
    sprintf(timeString, "%02d:%02d:%02d",
            localTime->tm_hour, localTime->tm_min, localTime->tm_sec);

    return timeString;
}

void *handleClient(void* arg) {
    ARGUMENTY *threadArgs = (ARGUMENTY *)arg;
    int newsockfd = threadArgs->newsockfd;
    char *username = threadArgs->username;
    int clientID = threadArgs->id;

    printf("Server thread : Client connected with username: %s, ID: %d\n", username, clientID);
    fflush(stdout);

    // inkrementovanie pocitadla pripojenych klientov
    pthread_mutex_lock(&counterMutex);
    connectedClients++;
    pthread_mutex_unlock(&counterMutex);

    // Cakanie kym sa vsetci klienti pripoja
    while (1) {
        pthread_mutex_lock(&counterMutex);
        if (connectedClients == 2) {
            pthread_mutex_unlock(&counterMutex);
            // Signal ze sa pripojili vsetci klienti
            write(newsockfd, "START_GAME", strlen("START_GAME") + 1);
            break;
        }
        pthread_mutex_unlock(&counterMutex);
        sleep(1);
    }

    char buffer[256];
    ssize_t n = read(newsockfd, buffer, sizeof(buffer));
    if (n < 0) {
        perror("Error reading from socket");
        exit(1);
    } else if (n == 0) {
        printf("Client disconnected: %s, ID: %d at: %s\n", username, clientID, getTime());
        fflush(stdout);

        free(arg);

    } else {
        buffer[n] = '\0';
        printf("Received data from client: %s\n", buffer);
        free(arg);
    }


    // Zatvorenie socketu
    close(newsockfd);

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    int sockfd, newsockfd;
    socklen_t cli_len;
    struct sockaddr_in serv_addr, cli_addr;
    char buffer[256];
    int n;



    printf("Server: The number of provided arguments is correct\n");
    fflush(stdout);

    if (argc < 2) {
        fprintf(stderr, "usage %s port\n", argv[0]);
        return 1;
    }

    //Inicializovanie struktury socketu
    bzero((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(atoi(argv[1]));

    // Vytvorenie socketu
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // Kontrola ci sa socket uspesne vytvoril
    if (sockfd < 0) {
        perror("Error creating socket");
        return 1;
    }
    printf("Server: created the socket\n");
    fflush(stdout);

    // bindovanie socketu
    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Error binding socket address");
        return 2;
    }
    printf("Server: bound the socket\n");
    fflush(stdout);

    // pocuvanie prichadzajucich pripojeni
    listen(sockfd, 5);
    cli_len = sizeof(cli_addr);

    // Spravovanie prichadzajucich spojeni
    while (1) {
        // Akceptovanie spojenia
        newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &cli_len);
        if (newsockfd < 0) {
            perror("ERROR on accept");
            continue;  // preskocenie do dalsej iteracie
        }
        printf("Server: accepting connection from client\n");
        fflush(stdout);

        ARGUMENTY *arg = (ARGUMENTY *)malloc(sizeof(ARGUMENTY));
        if (arg == NULL) {
            perror("Error allocating memory for ARGUMENTY");
            close(newsockfd);
            continue;
        }
        // Nacitanie username a id z klienta
        bzero(buffer, sizeof(buffer));
        n = read(newsockfd, buffer, sizeof(buffer) - 1);
        if (n < 0) {
            perror("Error reading username and ID from socket");
            close(newsockfd);
            free(arg);
            continue;  // preskocenie do dalsej iteracie
        }

        // parsovanie prijatej spravy na ziskanie mena a id
        if (sscanf(buffer, "%255[^,], ID: %d", arg->username, &(arg->id)) != 2) {
            fprintf(stderr, "Error parsing username and ID\n");
            close(newsockfd);
            free(arg);
            continue;  // preskocenie do dalsej iteracie
        }


        char *timeString = getTime();

        printf("Server: received username from client %s, ID: %d   at : %s \n", arg->username, arg->id, timeString);
        fflush(stdout);

        // uvolnenie alokovanej pamate
        free(timeString);
        // free(arg);
        arg->newsockfd = newsockfd;

        // vytvorenie noveho vlakna pre  spravovanie klienta
        pthread_t vlakno;
        if (pthread_create(&vlakno, NULL, handleClient, (void *)arg) != 0) {
            perror("Error creating thread");
            close(newsockfd);
            free(arg);
            continue;
        }

        // odpojenie vlakna
        pthread_detach(vlakno);
    }

    // uzatvorenie socketu
    close(sockfd);

    return 0;
}