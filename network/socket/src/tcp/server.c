#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<stdio.h>

int main(int argc, char** argv) {

    if(argc != 3) {
        fprintf(stderr, "%s server_ipv4 server_port\n", argv[0]);
        exit(1);
    }
    char buffer[200];
    int server_sd, client_sd;
    const char* server_ip = argv[1];
    uint16_t server_port = atoi(argv[2]);
    socklen_t addr_len = sizeof(struct sockaddr);
    struct sockaddr_in server_addr, client_addr;
    

    if( (server_sd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
        perror("Error: create socket\n");
        exit(1);
    }
    
    memset(&server_addr, 0, addr_len);
    if(inet_pton(AF_INET, server_ip, &server_addr.sin_addr) < 0) {
        perror("Error: convert server IP\n");
        exit(1);
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);

    if( bind(server_sd, (struct sockaddr*) &server_addr, addr_len) < 0 ) {
        perror("Error: bind address\n");
        exit(1);
    }

    if( listen(server_sd, 3) < 0 ) {
        perror("Error: listen requests\n");
        exit(1);
    }
    
    if((client_sd = accept(server_sd, (struct sockaddr*) &client_addr, &addr_len)) < 0) {
        perror("Error: accept requests\n");
        exit(1);
    }
    
    strcpy(buffer, "Good-morning");
    if(send(client_sd, buffer, sizeof(buffer), 0) < 0) {
        perror("Error: send messages\n");
        exit(1);
    }
    printf("Success: send message from server to client\n");

    
    if(recv(client_sd, buffer, sizeof(buffer), 0) < 0) {
        perror("Error: receive messages\n");
        exit(1);
    }
    printf("Success: receive message from client to server\n");
    printf("Message: %s\n", buffer);

    close(client_sd);
    close(server_sd);

    return 0;
}