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

    if( connect(sd, (struct sockaddr*) &server_addr, server_addr.sin_len) < 0 ) {
        perror("Error: connect server\n");
        exit(1);
    }
    
    char recieved_msg[200];
    int recieved_len;
    if( (recieved_len = recv(sd, recieved_msg, sizeof(recieved_msg), 0)) < 0 ) {
        perror("Error: receive message at client\n");
        exit(1);
    }
    printf("Message from server to client\n");
    printf("Total length: %d\n Total message: %s\n", recieved_len, recieved_msg);

    const char* sent_msg = "From client to server";
    int sent_len;
    if( (sent_len = send(sd, sent_msg, strlen(sent_msg), 0)) < 0 ) {
        perror("Error: send message at client\n");
        exit(1);
    }
    printf("Message from client to server\n");
    printf("Total length: %d\n Total message: %s\n", sent_len, sent_msg);

    if( close(sd) < 0 ) {
        perror("Error: close socket\n");
        exit(1);
    }

    return 0;
}