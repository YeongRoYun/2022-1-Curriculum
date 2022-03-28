#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<stdio.h>

#define IP "192.168.0.3"
#define PORT 3000

int main(int argc, char** argv) {

    int sd;

    if( (sd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
        perror("Error: create socket\n");
        exit(1);
    }
    struct sockaddr_in server_addr;
    memset((char*) &server_addr, '\0', sizeof(struct sockaddr_in));
    if( inet_pton(AF_INET, IP, &server_addr.sin_addr) < 0 ) {
        perror("Error: convert ip\n");
        exit(1);
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = PORT;
    server_addr.sin_len = sizeof(struct sockaddr_in);

    if( bind(sd, (struct sockaddr*) &server_addr, server_addr.sin_len) < 0 ) {
        perror("Error: bind address\n");
        exit(1);
    }

    if( listen(sd, 3) < 0 ) {
        perror("Error: listen requests\n");
        exit(1);
    }

    int client_sd;
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(struct sockaddr);
    while(1) {
        if( (client_sd = accept(sd, (struct sockaddr*) &client_addr, &addr_len)) < 0 ) {
            perror("disconnecting...\n");
            continue;
        }
        else {
            break;
        }
    }

    const char* sent_msg = "From server to client";
    int sent_len;
    if( (sent_len = send(client_sd, sent_msg, strlen(sent_msg), 0)) < 0 ) {
        perror("Error: send message at server\n");
        exit(1);
    }
    printf("Message from server to client\n");
    printf("Total length: %d\n Total message: %s\n", sent_len, sent_msg);

    char recieved_msg[200];
    socklen_t recieved_len;
    if( (recieved_len = recv(client_sd, recieved_msg, sizeof(recieved_msg), 0)) < 0 ) {
        perror("Error: receive message at server\n");
        exit(1);
    }
    printf("Message from client to server\n");
    printf("Total length: %d\n Total message: %s\n", recieved_len, recieved_msg);

    if( (close(client_sd) < 0) && (close(sd) < 0) ) {
        perror("Error: close socket\n");
        exit(1);
    }

    return 0;
}