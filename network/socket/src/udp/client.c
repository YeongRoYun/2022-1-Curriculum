#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<stdio.h>

int main(int argc, char** argv) {

    if(argc != 5) {
        fprintf(stderr, "%s server_ipv4 server_port client_ipv4 client_port\n", argv[0]);
        exit(1);
    }
    char buffer[200];
    int client_sd;
    const char* server_ip = argv[1];
    uint16_t server_port = atoi(argv[2]);
    const char* client_ip = argv[3];
    uint16_t client_port = atoi(argv[4]);
    socklen_t addr_len = sizeof(struct sockaddr);
    struct sockaddr_in server_addr, client_addr;

    if((client_sd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Error: create socket\n");
        exit(1);
    }

    memset(&client_addr, 0, addr_len);
    if(inet_pton(AF_INET, client_ip, &client_addr.sin_addr) < 0) {
        perror("Error: convert client IPt\n");
        exit(1);
    }
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(client_port);

    if(bind(client_sd, (struct sockaddr*) &client_addr, addr_len) < 0) {
        perror("Error: bind address\n");
        exit(1);
    }

    memset(&server_addr, 0, addr_len);
    if(inet_pton(AF_INET, server_ip, &server_addr.sin_addr) < 0) {
        perror("Error: convert server IP\n");
        exit(1);
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);

    if(recvfrom(client_sd, buffer, sizeof(buffer), 0, (struct sockaddr*) &server_addr, &addr_len) < 0){
        perror("Error: receive messages\n");
        exit(1);
    }
    printf("Success: receive message from server to client\n");
    printf("Message: %s\n", buffer);

    strcpy(buffer, "Good-night");
    if(sendto(client_sd, buffer, sizeof(buffer), 0, (struct sockaddr*) &server_addr, addr_len) < 0){
        perror("Error: send messages\n");
        exit(1);
    }
    printf("Success: send message from client to server\n");

    close(client_sd);

    return 0;
}
