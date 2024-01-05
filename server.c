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
    int n;
    char buffer[256];
} ARGUMENTY;

void *handleClient(void* arg) {
    printf("Server: som vo vlakne\n");
    fflush(stdout);

    ARGUMENTY *threadArgs = (ARGUMENTY *)arg;
    int newsockfd = threadArgs->newsockfd;
    char *username = threadArgs->username;
    printf("Server: zapisal som potrebne informacie\n");
    fflush(stdout);
    //vynulovany buffer
    bzero(threadArgs->buffer,256);
    //precitam a ulozim informcie do buffra - max 255 bajtov
    threadArgs->n = read(newsockfd, threadArgs->buffer, 255);

    // Your existing code for handling a single client
    printf("Here is the message: %s\n", threadArgs->buffer);

    const char* msg = "I got your message";
    //mag + 1 ,pretoze je tu este /0 na ukoncenie
    threadArgs->n = write(newsockfd, msg, strlen(msg)+1);
    if (threadArgs->n < 0)
    {
        perror("Error writing to socket");
    }
    // For example, printing the username and a message
    printf("Client connected with username: %s\n", username);

    // Free the memory allocated for the threadArgs
    free(threadArgs);

    // Close the socket for this client
    close(newsockfd);

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    int sockfd, newsockfd;
    socklen_t cli_len;
    struct sockaddr_in serv_addr, cli_addr;
    char username[256];
    int n;
    char buffer[256];

    printf("Server: pocet zadanych argumentov je spravny\n");
    fflush(stdout);

    if (argc < 2) {
        fprintf(stderr,"usage %s port\n", argv[0]);
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
    printf("Server: vytvoril som socket\n");
    fflush(stdout);

    // Bind the socket
    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Error binding socket address");
        return 2;
    }
    printf("Server: bindol som socket\n");
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
        printf("Server: prijimam pripojenie klienta\n");
        fflush(stdout);

        ARGUMENTY *arg = (ARGUMENTY *)malloc(sizeof(ARGUMENTY));
        if (arg == NULL) {
            perror("Error allocating memory for ARGUMENTY");
            close(newsockfd);
            continue;
        }

        // Read the username from the client
        // char username[256];
        bzero(username, sizeof(username));

        n = read(newsockfd, username, sizeof(username)-1);
        if (n < 0) {
            perror("Error reading username from socket");
            close(newsockfd);
            free(arg);
            continue;  // Continue to the next iteration
        }
        printf("Server: citam username klienta %s : \n" ,username );
        fflush(stdout);

        // Populate the ARGUMENTY structure
        arg->newsockfd = newsockfd;
        strncpy(arg->username, username, sizeof(arg->username));
        strncpy(arg->buffer, buffer, sizeof(arg->buffer));

        // Create a new thread to handle the client
        printf("Server: idem vytvorit vlakno \n" );
        fflush(stdout);

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

