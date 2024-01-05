#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "hra.h"


int main(int argc, char *argv[])
{


    int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent* server; //informacie o serveri

    char buffer[256];

    //pocet argumentov
    if (argc < 3)
    {
        fprintf(stderr,"usage %s hostname port\n", argv[0]);
        return 1;
    }
    printf("klient :pocet argumentov je spravny\n");
    //informacie o serveri na ktory sa idem pripajat ziskam pomocou funkcie gethostbyname
    server = gethostbyname(argv[1]);
    if (server == NULL)
    {
        fprintf(stderr, "Error, no such host\n");
        return 2;
    }

    printf("klient :ziskal som informacie o serveri\n");

    //informacie o serveri na ktory sa pripajam
    bzero((char*)&serv_addr, sizeof(serv_addr));
    //internetove sockety
    serv_addr.sin_family = AF_INET;
    bcopy(
            (char*)server->h_addr, //ip adresa serveru
            (char*)&serv_addr.sin_addr.s_addr,
            server->h_length
    );

    printf("klient :ziskal som informacie o serveri 2\n");
    //port
    serv_addr.sin_port = htons(atoi(argv[2]));
    printf("klient :nastavil som port\n");
    //vytvoenie socketu
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Error creating socket");
        return 3;
    }

    printf("klient :vytvorenie socketu prebehlo uspesne\n");

    //cez ktory sockety sa kam pripajam ,cakam na odpoved servera
    if(connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("Error connecting to socket");
        return 4;
    }

    printf("klient :uspesne som sa pripojil na server\n");

    //kod programu
    char* user = getenv("USER");
    bzero(buffer,256);
    n =  write(sockfd, user, strlen(user));
    if (n < 0)
    {
        perror("Error writing username to socket");
        return 5;
    }
    printf("klient : zapisal som uzivatelske meno : %s\n" , user);
    //hra();
    printf("Please enter a message: ");
    bzero(buffer,256);
    fgets(buffer, 255, stdin);





    n = write(sockfd, buffer, strlen(buffer));
    if (n < 0)
    {
        perror("Error writing to socket");
        return 5;
    }

    bzero(buffer,256);
    n = read(sockfd, buffer, 255);
    if (n < 0)
    {
        perror("Error reading from socket");
        return 6;
    }

    printf("%s\n",buffer);

    //uzavrenie socketu
    close(sockfd);

    return 0 ;
}