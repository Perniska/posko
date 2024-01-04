#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "hra.h"

int main(int argc, char *argv[])
{

    int sockfd, newsockfd;
    socklen_t cli_len;
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    char buffer[256];
    //skontrolujeme pocet argumentov ,ak je mensi ako 2 tak sme nezadali port

    if (argc < 2)
    {
        fprintf(stderr,"usage %s port\n", argv[0]);
        return 1;
    }

    //vycistenie struktury ktoru nastavujeme (typ socketov , rozsah ip adries a port)
    bzero((char*)&serv_addr, sizeof(serv_addr));
    //informacie o tom aký typ socketov chcem použiť (AF_INET -internetove sockety)
    serv_addr.sin_family = AF_INET;
    //odkial budem chcet pocuvat /primat spojenie -rozzsah ip adries alebo maska podsiete (mame nastaveny cely internet)
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    //informacia na akom porte pocuvam -htons (potrebujeme big endian a prevadzame little endian to big endian)
    serv_addr.sin_port = htons(atoi(argv[1]));

    //jednoznacne identifikovanie socketu
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    //ak unixove volanie zlyha tak vrati hodnotu mensiu ako 0
    if (sockfd < 0)
    {
        perror("Error creating socket");
        return 1;
    }

    //vytvorenie danej struktury
    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("Error binding socket address");
        return 2;
    }

    //pasivny socket - nie je urceny na komunikaciu s klietom ale na pripojenie
    //potom si server vytvori dalsi socket ktorym uz bude komunikovat s klientom
    listen(sockfd, 5); //v jednom okamziku sa mi moze pripojitmaximalne 5 klientov
    cli_len = sizeof(cli_addr);

    //tu sa caka kym sa niekto pripoji
    newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &cli_len);
    if (newsockfd < 0)
    {
        perror("ERROR on accept");
        return 3;
    }

    char username[256];
    bzero(username, sizeof(username));
    int nn = read(newsockfd, username, sizeof(username)-1);
    if (nn < 0) {
        perror("Error reading username from socket");
        return 4;
    }

// Print the username
    printf("Client connected with username: %s\n", username);
    hra();
    //vynulovany buffer
    bzero(buffer,256);
    //precitam a ulozim informcie do buffra - max 255 bajtov
    n = read(newsockfd, buffer, 255);



    //bajtov nesmie byt menej ako 1
    if (n < 0)
    {
        perror("Error reading from socket");
        return 4;
    }

    printf("Here is the message: %s\n", buffer);



    const char* msg = "I got your message";
    //mag + 1 ,pretoze je tu este /0 na ukoncenie
    n = write(newsockfd, msg, strlen(msg)+1);
    if (n < 0)
    {
        perror("Error writing to socket");
        return 5;
    }


    //ukoncenie komunikacie
    close(newsockfd);
    close(sockfd);

    return 0;
}
