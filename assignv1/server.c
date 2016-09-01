/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int check(char buff[], int len)
{
    for (int i = 0; i <= len; i++) {
        if (buff[i] == '\0') {
            return 1;
        }
    }
    return 0;
}

void dostuff(int sockfd)
{
    char buffer[3];
    int n;
    while (1) {
        bzero(buffer, sizeof(buffer));
        int n = read(sockfd, buffer, sizeof(buffer));
        if (n < 0) {
            error("ERROR reading from socket");
        }
        printf("%s", buffer);
        if (check(buffer, sizeof(buffer))) break;
    }
    bzero(buffer, sizeof(buffer));
    n = write(sockfd,"Message received\0",16);
    if (n < 0) {
        error("ERROR writing to socket");
    }
}

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error("ERROR opening socket");
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        error("ERROR on binding");
    }
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    while (1) {
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0) {
            error("ERROR on accept");
        }
        while (1) {
            dostuff(newsockfd);
        }
        close(newsockfd);
    }
    close(sockfd);
    return 0; 
}
