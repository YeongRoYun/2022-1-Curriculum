#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<stdio.h>

#include"../header.h"

#define SERVER_IP "192.168.0.3"
#define SERVER_PORT 4000
#define CLIENT_IP "192.168.0.3"
#define CLIENT_PORT 3000

int main(int argc, char** argv){

    int sd;
    int len;
    struct sockaddr_in client_addr, server_addr;
    struct tcp_header tcph;
    socklen_t addr_len = sizeof(struct sockaddr);
    size_t tcph_len = sizeof(struct tcp_header);

    memset(&client_addr, 0, addr_len);
    memset(&server_addr, 0, addr_len);
    memset(&tcph, 0, tcph_len);

    if((sd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP)) < 0) {
        perror("Error: socket\n");
        exit(1);
    }

    inet_pton(AF_INET, CLIENT_IP, &client_addr.sin_addr);
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(CLIENT_PORT);
    
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);

    if(bind(sd, (struct sockaddr*) &client_addr, addr_len) < 0) {
        perror("Error: bind\n");
        exit(1);
    }

    recvfrom(sd, &tcph, tcph_len, 0, (struct sockaddr*) &server_addr, &addr_len);
    printf("success\n");

    if(close(sd) < 0) {
        perror("Error: exit socket at server");
        exit(1);
    }

    return 0;
}
