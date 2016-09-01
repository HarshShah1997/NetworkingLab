/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <math.h>

typedef struct Data {
    int a;
    float b;
} Data;

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void dostuff(int sockfd, int sockfd2)
{
    Data data;
    //bzero(buffer,256);
    int n = read(sockfd, &data, sizeof(data));
    if (n < 0) {
        error("ERROR reading from socket");
    }
    printf("Here is the message: %d %f\n", data.a, data.b);
    int temp = data.a;
    data.a = (int) data.b;
    data.b = temp;
    n = write(sockfd2, &data, sizeof(data));
    if (n < 0) {
        error("ERROR writing to socket");
    }
}

int main(int argc, char *argv[])
{
    signal(SIGCHLD,SIG_IGN);
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr, cli_addr2;
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
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if (newsockfd < 0) {
        error("ERROR on accept");
    }
    int newsockfd2 = accept(sockfd, (struct sockaddr *) &cli_addr2, &clilen);
    dostuff(newsockfd, newsockfd2);
    close(newsockfd);
    close(sockfd);
    return 0; 
}
