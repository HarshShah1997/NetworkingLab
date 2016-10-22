#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <string>
#include <map>
#include <iostream>
#include <vector>
using namespace std;

map<string, int> nametable;

int setup(int argc, char *argv[]);

void error(const char *msg) 
{
    perror(msg);
    exit(EXIT_FAILURE);
}

string recv_message(int sockfd)
{
    char buffer[256];
    bzero(buffer, 256);
    ssize_t n = recv(sockfd, buffer, sizeof(buffer), 0);
    if (n <= 0) {
        cout << "ERROR receiving from socket" << endl;
        return "EOF";
    }
    string tempstr (buffer);
    return tempstr;
}

void send_message(int sockfd, string msg)
{
    ssize_t n = send(sockfd, msg.c_str(), msg.length() + 1, 0);
    if (n < 0) {
        error("ERROR sending to socket");
    }
}

void *handle_client(void *sfd)
{
    int clisockfd = *(int *)sfd;
    send_message(clisockfd, "Connected, enter your name");
    string cliname = recv_message(clisockfd);
    cout << cliname << " connected" << endl;
    nametable[cliname] = clisockfd;

    while (1) {
        string message = recv_message(clisockfd);
        cout << cliname << ": " << message << endl;
        if (message == "EOF") {
            nametable.erase(cliname);
            break;
        } else if (message == "list") {
            send_message(clisockfd, "List of clients:\n");
            for (map<string, int>::iterator it = nametable.begin(); it != nametable.end(); it++) {
                send_message(clisockfd, it -> first + "\n");
            }
        } else if (message.find(":") == string::npos) {
            send_message(clisockfd, "Invalid command\n");
        } else {
            string recvname = message.substr(0, message.find(":"));
            if (nametable.find(recvname) == nametable.end()) {
                send_message(clisockfd, "No such client\n");
            } else {
                int recvsockfd = nametable[recvname];
                string org_msg = cliname + ":" + message.substr(message.find(":") + 1, string::npos);
                send_message(recvsockfd, org_msg + "\n");
            }
        }
    }
}

int main(int argc, char *argv[])
{
    int sockfd = setup(argc, argv);
    struct sockaddr_in cli_addr;
    listen(sockfd, 5);
    socklen_t clilen = sizeof(cli_addr);

    while (1) {
        int clisockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (clisockfd < 0) {
            error("ERROR on accept");
        }
        pthread_t tid;
        pthread_create(&tid, NULL, handle_client, (void *)&clisockfd);
    }
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

