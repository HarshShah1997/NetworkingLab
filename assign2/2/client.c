#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define MAXBUFLEN 100000

void error(char *msg) 
{
    perror(msg);
    exit(0);
}

void dostuff(int sockfd)
{
    char buffer[MAXBUFLEN + 1];
    int n;
    bzero(buffer, MAXBUFLEN);
    printf("Enter file name: ");
    char inputFile[256];
    scanf("%s", inputFile);
    FILE *in = fopen(inputFile, "r");
    if (in == NULL) {
        error("ERROR Reading file");
    }
    size_t newLen = fread(buffer, sizeof(char), MAXBUFLEN, in);
    buffer[newLen++] = '\0';
    fclose(in);
    n = write(sockfd, buffer, strlen(buffer));
    if (n < 0) {
        error("ERROR writing to socket");
    }
    bzero(buffer, MAXBUFLEN);
    n = read(sockfd, buffer, sizeof(buffer));
    if (n < 0) {
        error("ERROR reading from socket");
    }
    printf("%s\n", buffer);
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
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        error("ERROR connection");
    } else {
        dostuff(sockfd);
    }
    close(sockfd);
    return 0;
}

