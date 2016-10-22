#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t lock;

void error(char *msg) 
{
    perror(msg);
    exit(0);
}

void *send_to_server(void *args)
{
    int sockfd = *(int *)args;
    char buffer[256];
    bzero(buffer, sizeof(buffer));

    while (1) {
        fgets(buffer, sizeof(buffer) - 1, stdin);

        pthread_mutex_lock(&lock);
        int n = write(sockfd, buffer, strlen(buffer));
        if (n < 0) {
            error("ERROR writing to socket");
        }
        pthread_mutex_unlock(&lock);
    }
}

void *receive_from_server(void *args)
{
    int sockfd = *(int *)args;
    char buffer[256];
    bzero(buffer, sizeof(buffer));
    while (1) {
        int n = read(sockfd, buffer, sizeof(buffer) - 1);
        if (n < 0) {
            error("ERROR reading from socket");
        }
        pthread_mutex_lock(&lock);
        printf("%s", buffer);
        pthread_mutex_unlock(&lock);
    }
}



int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    if (argc < 3) {
        fprintf(stderr, "Usage %s hostname port\n", argv[0]);
        exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error("ERROR opening socket");
    }
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr, "ERROR no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    /*
       bcopy(char *s1, char *s2, int length)
       Copies from a string from s1 to s2 with specified length.
     */
    bcopy((char *)server -> h_addr, 
            (char *)&serv_addr.sin_addr.s_addr, server -> h_length);
    serv_addr.sin_port = htons(portno);

    if (pthread_mutex_init(&lock, NULL) != 0) {
        error("ERROR creating mutex");
    }

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        error("ERROR connection");
    } else {
        pthread_t pid[2];
        pthread_create(&pid[0], NULL, send_to_server, &sockfd);
        pthread_create(&pid[1], NULL, receive_from_server, &sockfd);
        pthread_join(pid[0], NULL);
        pthread_join(pid[1], NULL);
    }
    close(sockfd);
    pthread_mutex_destroy(&lock);
    return 0;
}

