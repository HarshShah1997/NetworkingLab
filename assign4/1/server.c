#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>

int setup(int argc, char *argv[]);

void error(const char *msg) 
{
    perror(msg);
    exit(EXIT_FAILURE);
}

void *recv_message(void *sfd)
{
    int sockfd = *(int *)sfd;
    while (1) {
        char buffer[256];
        bzero(buffer, 256);
        ssize_t n = recv(sockfd, buffer, sizeof(buffer), 0);
        if (n <= 0) {
            error("ERROR receiving from socket");
        }
        printf("%s\n", buffer);
    }
}

void *send_message(void *sfd)
{
    int sockfd = *(int *)sfd;
    while (1) {
        char buffer[256];
        bzero(buffer, 256);
        scanf("%s", buffer);
        ssize_t n = send(sockfd, buffer, sizeof(buffer), 0);
        if (n < 0) {
            error("ERROR sending to socket");
        }
    }
}

int main(int argc, char *argv[])
{
    int sockfd = setup(argc, argv);
    struct sockaddr_in cli_addr;
    listen(sockfd, 5);
    socklen_t clilen = sizeof(cli_addr);

    int newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if (newsockfd < 0) {
        error("ERROR on accept");
    }

    pthread_t sid, rid;
    pthread_create(&rid, NULL, recv_message, (void *)&newsockfd);
    pthread_create(&sid, NULL, send_message, (void *)&newsockfd);

    pthread_join(rid, NULL);
    pthread_join(sid, NULL);

    close(sockfd);
    return 0;
}

int setup(int argc, char *argv[])
{
    signal(SIGCHLD, SIG_IGN);
    int sockfd, portno;
    struct sockaddr_in serv_addr;
    if (argc < 2) {
        error("ERROR no port provided");
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
    return sockfd;
}

