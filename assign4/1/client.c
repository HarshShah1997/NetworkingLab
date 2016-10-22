#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>
#include <pthread.h>

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

    pthread_t rid, sid;

    pthread_create(&rid, NULL, recv_message, (void *)&sockfd);
    pthread_create(&sid, NULL, send_message, (void *)&sockfd);

    pthread_join(rid, NULL);
    pthread_join(sid, NULL);

    close(sockfd);
    return 0;
}

int setup(int argc, char *argv[])
{
    int sockfd, portno;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    if (argc < 3) {
        error("ERROR no hostname or port provided");
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error("ERROR opening socket");
    }
    portno = atoi(argv[2]);
    bzero((char *) &serv_addr, sizeof(serv_addr));
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        error("ERROR no such host");
    }
    serv_addr.sin_family = AF_INET;
    bcopy((char *) server -> h_addr, (char *) &serv_addr.sin_addr.s_addr, 
            server -> h_length);

    serv_addr.sin_port = htons(portno);
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        error("ERROR on connecting");
    }
    return sockfd;
}

