#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<stdio.h>

int main(int argc, char** argv) {
    int sd;
    struct sockaddr_in addr;
    uint8_t* buffer;

    sd = socket(AF_INET, SOCK_RAW, 13);
    
    memset(&addr, 0, sizeof(addr));
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
    addr.sin_family = AF_INET;
    addr.sin_port = 0;
    
    buffer = (uint8_t*)malloc(4);
    memcpy(buffer, "Yes", 4);

    sendto(sd, buffer, 4, 0, (struct sockaddr*) &addr, sizeof(addr));
    
    close(sd);

    return 0;
}