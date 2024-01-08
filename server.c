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
    int id;  // Unique ID for the client
} ARGUMENTY;

int connectedClients = 0;  // Counter for connected clients
pthread_mutex_t counterMutex = PTHREAD_MUTEX_INITIALIZER;  // Mutex to protect counter

char* getTime() {
    time_t currentTime;
    struct tm *localTime;

    time(&currentTime);
    localTime = localtime(&currentTime);

    // Allocate memory for the time string
    char *timeString = (char *)malloc(9); // HH:MM:SS\0

    // Format the time string
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

    // Increment the connected clients counter
    pthread_mutex_lock(&counterMutex);
    connectedClients++;
    pthread_mutex_unlock(&counterMutex);

    // Wait for all clients to connect
    while (1) {
        pthread_mutex_lock(&counterMutex);
        if (connectedClients == 2) {
            pthread_mutex_unlock(&counterMutex);
            // Signal both clients that the game can start
            write(newsockfd, "START_GAME", strlen("START_GAME") + 1);
            break;  // Break the loop when 2 clients are connected
        }
        pthread_mutex_unlock(&counterMutex);
        sleep(1);  // Sleep to avoid busy-waiting
    }

    char buffer[256];
    ssize_t n = read(newsockfd, buffer, sizeof(buffer));
    if (n < 0) {
        perror("Error reading from socket");
        exit(1);
    } else if (n == 0) {
        // Print the message
        printf("Client disconnected: %s, ID: %d at: %s\n", username, clientID, getTime());
        fflush(stdout);

        free(arg);

    } else {
        buffer[n] = '\0';  // Null-terminate the received data
        printf("Received data from client: %s\n", buffer);
        // Parse the received data as needed
        // Free the allocated memory
        free(arg);
    }


    // Close the socket for this client
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

    // Initialize socket structure
    bzero((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(atoi(argv[1]));

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // Check if socket creation was successful
    if (sockfd < 0) {
        perror("Error creating socket");
        return 1;
    }
    printf("Server: created the socket\n");
    fflush(stdout);

    // Bind the socket
    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Error binding socket address");
        return 2;
    }
    printf("Server: bound the socket\n");
    fflush(stdout);

    // Listen for incoming connections
    listen(sockfd, 5);
    cli_len = sizeof(cli_addr);

    // Accept and handle incoming connections in an infinite loop
    while (1) {
        // Accept a connection
        newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &cli_len);
        if (newsockfd < 0) {
            perror("ERROR on accept");
            continue;  // Continue to the next iteration
        }
        printf("Server: accepting connection from client\n");
        fflush(stdout);

        ARGUMENTY *arg = (ARGUMENTY *)malloc(sizeof(ARGUMENTY));
        if (arg == NULL) {
            perror("Error allocating memory for ARGUMENTY");
            close(newsockfd);
            continue;
        }
        // Read the username and ID from the client
        bzero(buffer, sizeof(buffer));
        n = read(newsockfd, buffer, sizeof(buffer) - 1);
        if (n < 0) {
            perror("Error reading username and ID from socket");
            close(newsockfd);
            free(arg);
            continue;  // Continue to the next iteration
        }

        // Parse the received message to extract username and ID
        if (sscanf(buffer, "%255[^,], ID: %d", arg->username, &(arg->id)) != 2) {
            fprintf(stderr, "Error parsing username and ID\n");
            close(newsockfd);
            free(arg);
            continue;  // Continue to the next iteration
        }

        // Get the time string
        char *timeString = getTime();

        printf("Server: received username from client %s, ID: %d   at : %s \n", arg->username, arg->id, timeString);
        fflush(stdout);

        // Free the allocated memory
        free(timeString);
        // free(arg);
        // Populate the ARGUMENTY structure
        arg->newsockfd = newsockfd;

        // Create a new thread to handle the client
        pthread_t vlakno;
        if (pthread_create(&vlakno, NULL, handleClient, (void *)arg) != 0) {
            perror("Error creating thread");
            close(newsockfd);
            free(arg);
            continue;  // Continue to the next iteration
        }

        // Detach the thread to allow it to clean up after completion
        pthread_detach(vlakno);
    }

    // Close the main socket
    close(sockfd);

    return 0;
}