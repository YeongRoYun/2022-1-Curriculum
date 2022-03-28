#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<stdio.h>

int main(int argc, char** argv) {
    
    if(argc != 3) {
        perror("%s server_ipv4 server_port\n");
        exit(1);
    }
    char buffer[200];
    int sd;
    const char* ip = argv[1];
    uint16_t port = atoi(argv[2]);
    socklen_t addr_len = sizeof(struct sockaddr);
    struct sockaddr_in server_addr;
    

    if( (sd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
        perror("Error: create socket\n");
        exit(1);
    }

    memset(&server_addr, 0, addr_len);
    if( inet_pton(AF_INET, ip, &server_addr.sin_addr) < 0 ) {
        perror("Error: convert IP\n");
        exit(1);
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    
    if( connect(sd, (struct sockaddr*) &server_addr, addr_len) < 0 ) {
        perror("Error: connect server\n");
        exit(1);
    }
    
    if(recv(sd, buffer, sizeof(buffer), 0) < 0) {
        perror("Error: receive messagest\n");
        exit(1);
    }
    printf("Success: receive message from server to client\n");
    printf("Message: %s\n", buffer);

    strcpy(buffer, "Good-night");
    if(send(sd, buffer, sizeof(buffer), 0) < 0 ) {
        perror("Error: send message at client\n");
        exit(1);
    }
    printf("Success: send message from client to server\n");
    
    close(sd);

    return 0;
}