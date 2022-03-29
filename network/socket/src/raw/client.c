#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<stdio.h>

int main(int argc, char** argv) {
    
    if(argc != 3) {
        fprintf(stderr, "%s server_ipv4 server_port\n", argv[0]);
        exit(1);
    }
    char buffer[200];
    int sd;
    const char* ip = argv[1];
    uint16_t port = atoi(argv[2]);
    socklen_t addr_len = sizeof(struct sockaddr);
    struct sockaddr_in server_addr;
    
    int sd, len;
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    uint8_t* buffer;

    sd = socket(AF_INET, SOCK_RAW, 13);
    
    memset(&addr, 0, sizeof(addr));
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
    addr.sin_family = AF_INET;
    addr.sin_port = 0;
    
    buffer = (uint8_t*)malloc(200);

    len = recvfrom(sd, buffer, 200, 0, (struct sockaddr*) &addr, &addr_len);
    printf("len: %d\n", len);
    for(int i = 0; i < len; ++i)
        printf("%02X ", buffer[i]);
    for(int i = 20; i < 24; ++i)
        printf("%c", buffer[i]);
    close(sd);
    return 0;
}