#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<stdio.h>

#define SERVER_IP "192.168.0.3"
#define SERVER_PORT 4000
#define CLIENT_IP "192.168.0.3"
#define CLIENT_PORT 3000

int main(int argc, char** argv) {

    int sd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(struct sockaddr);
    char buf[200], msg[200];

    if((sd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Error: create server socket at server\n");
        exit(1);
    }
    memset(&server_addr, 0, sizeof(struct sockaddr_in));
    if(inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) < 0) {
        perror("Error: convert server ip at server\n");
        exit(1);
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);

    if(bind(sd, (struct sockaddr*) &server_addr, addr_len) < 0) {
        perror("Error: bind server at server");
        exit(1);
    }

    memset(&client_addr, 0, sizeof(struct sockaddr_in));
    if(inet_pton(AF_INET, CLIENT_IP, &client_addr.sin_addr) < 0) {
        perror("Error: convert client ip at server\n");
        exit(1);
    }
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(CLIENT_PORT);

    strcpy(msg, "From server to client");
    while(sendto(sd, msg, strlen(msg), 0, (struct sockaddr*) &client_addr, addr_len) < 0);
    printf("Success: send message from server to client\n");

    while(recvfrom(sd, buf, sizeof(buf), 0, (struct sockaddr*) &client_addr, &addr_len) < 0);
    printf("Success: receive message from client to server\n");
    printf("Message: %s\n", buf);

    if(close(sd) < 0) {
        perror("Error: exit socket at server");
        exit(1);
    }

    return 0;
}
