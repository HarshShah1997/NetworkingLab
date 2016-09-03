#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <pthread.h>

pthread_mutex_t lock;

int clients[5];
int clients_size = 0;

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void *read_from_client(void *args)
{
    int sockfd = *((int *) args);
    char buffer[256];
    while (1) {
        bzero(buffer, sizeof(buffer));
        int n = read(sockfd, buffer, sizeof(buffer) - 1);
        if (n < 0) {
            error("ERROR reading from socket");
        }
        pthread_mutex_lock(&lock);
        printf("%s", buffer);
        pthread_mutex_unlock(&lock);

    }
}

void *send_to_client(void * args)
{
    int sockfd = *((int *)args);
    char buffer[256];
    bzero(buffer, sizeof(buffer));

    while (1) {
        fgets(buffer, sizeof(buffer) - 1, stdin);

        pthread_mutex_lock(&lock);
        int n = write(sockfd, buffer, sizeof(buffer) - 1);
        if (n < 0) {
            error("ERROR writing to socket");
        }
        pthread_mutex_unlock(&lock);
    }
}

int main(int argc, char *argv[])
{
    signal(SIGCHLD,SIG_IGN);
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

    pthread_t tid[2];
    if (pthread_mutex_init(&lock, NULL) != 0) {
        error("ERROR Mutex initialization failed");
    }

    while (1) {
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0) {
            error("ERROR on accept");
        }
        pthread_create(&tid[0], NULL, read_from_client, &newsockfd);
        pthread_create(&tid[1], NULL, send_to_client, &newsockfd);
        pthread_join(tid[0], NULL);
        pthread_join(tid[1], NULL);


    }
    pthread_mutex_destroy(&lock);
    return 0; 
}
