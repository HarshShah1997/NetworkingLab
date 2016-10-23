#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>
#include <pthread.h>
#include <string>
#include <iostream>
using namespace std;

int setup(int argc, char *argv[]);

void error(const char *msg) 
{
    perror(msg);
    exit(EXIT_FAILURE);
}
void *recv_message_from_server(void *sfd)
{
    int sockfd = *(int *)sfd;
    while (1) {
        char buffer[256];
        bzero(buffer, 256);
        ssize_t n = recv(sockfd, buffer, sizeof(buffer), 0);
        if (n <= 0) {
            error("ERROR receiving from socket");
        }
        write(fileno(stdout), "< ", 2);
        write(fileno(stdout), buffer, sizeof(buffer));
    }
}

void *send_message_to_server(void *sfd)
{
    int sockfd = *(int *)sfd;
    while (1) {
        string buffer;
        getline(cin, buffer);
        ssize_t n = send(sockfd, buffer.c_str(), buffer.length(), 0);
        if (n < 0) {
            error("ERROR sending to socket");
        }
    }
}

string recv_message(int sockfd)
{
    char buffer[256];
    bzero(buffer, 256);
    ssize_t n = recv(sockfd, buffer, sizeof(buffer), 0);
    if (n <= 0) {
        error("ERROR receiving from socket");
    }
    return string(buffer);
}

void send_message(int sockfd, string msg)
{
    ssize_t n = send(sockfd, msg.c_str(), msg.length() + 1, 0);
    if (n < 0) {
        error("ERROR sending to socket");
    }
}

int main(int argc, char *argv[])
{
    int sockfd = setup(argc, argv);
    string greetmsg = recv_message(sockfd);
    cout << greetmsg << endl;
    string name;
    cin >> name;
    send_message(sockfd, name);

    pthread_t rid, sid;
    pthread_create(&rid, NULL, recv_message_from_server, (void *)&sockfd);
    pthread_create(&sid, NULL, send_message_to_server, (void *)&sockfd);

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

