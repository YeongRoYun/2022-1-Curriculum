#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<stdio.h>

#include "header.h"

int main(int argc, char** argv){

    if(argc != 5) {
        fprintf(stderr, "%s server_ipv4 server_port client_ipv4 client_port\n", argv[0]);
        exit(1);
    }
    int server_sd;
    int on = 1;
    const char* server_ip = argv[1];
    uint16_t server_port = atoi(argv[2]);
    const char* client_ip = argv[3];
    uint16_t client_port = atoi(argv[4]);
    socklen_t addr_len = sizeof(struct sockaddr);
    struct sockaddr_in server_addr, client_addr;
    struct tcp_header tcph;
    struct ip_header iph;
    struct pseudo_header psdh;
    size_t iph_len = sizeof(struct ip_header);
    size_t tcph_len = sizeof(struct tcp_header);
    size_t psdh_len = sizeof(struct pseudo_header);
    uint16_t* tmph;
    uint8_t* buffer;


    if((server_sd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP)) < 0) {
        perror("Error: create socket\n");
        exit(1);
    }

    if(setsockopt(server_sd, IPPROTO_IP, IP_HDRINCL, &on, sizeof(on)) < 0) {
		perror("Error: set sockopt\n");
        exit(1);
	}
    
    memset(&server_addr, 0, addr_len);
    if(inet_pton(AF_INET, server_ip, &server_addr.sin_addr) < 0) {
        perror("Error: convert server IP\n");
        exit(1);
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);

    if(bind(server_sd, (struct sockaddr*) &server_addr, addr_len) < 0) {
        perror("Error: bind address\n");
        exit(1);
    }

    memset(&client_addr, 0, addr_len);
    if(inet_pton(AF_INET, client_ip, &client_addr.sin_addr) < 0) {
        perror("Error: convert client IPt\n");
        exit(1);
    }
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(client_port);  
    
    memset(&iph, 0, iph_len);
    memset(&tcph, 0, tcph_len);
    memset(&psdh, 0, psdh_len);

    iph.version = 4;
    iph.ihl = 5;
    iph.tos = 0;
    iph.totlen = htons(iph_len + tcph_len);
    iph.id = 0;
    iph.flags = 2;
    iph.offset = 0;
    iph.ttl = 152;
    iph.protocol = IPPROTO_TCP;
    iph.source = server_addr.sin_addr.s_addr;
    iph.dest = client_addr.sin_addr.s_addr;
    iph.checksum = 0;
    iph.checksum = checksum((uint16_t*) &iph, iph_len);
    

    tcph.source = server_addr.sin_port;
    tcph.dest = client_addr.sin_port;
    tcph.sequence = 0;
    tcph.acknowledge = 1;
    tcph.offset = 5;
    tcph.reserved = 0;
    tcph.urg = 0;
    tcph.ack = 0;
    tcph.psh = 0;
    tcph.rst = 0;
    tcph.syn = 0;
    tcph.fin = 1;
    tcph.window = htons(512);

    // iph.version = (htons(4) >> 12);
    // iph.ihl = (htons(5) >> 12);
    // iph.tos = 0;
    // iph.totlen = htons(iph_len + tcph_len);
    // iph.id = 0;
    // iph.flags = (htons(2) >> 13);
    // iph.offset = 0;
    // iph.ttl = (htons(152) >> 8);
    // iph.protocol = (htons(IPPROTO_TCP) >> 8);
    // iph.source = server_addr.sin_addr.s_addr;
    // iph.dest = client_addr.sin_addr.s_addr;
    // iph.checksum = 0;
    // iph.checksum = checksum((uint16_t*) &iph, iph_len);
    

    // tcph.source = server_addr.sin_port;
    // tcph.dest = client_addr.sin_port;
    // tcph.sequence = 0;
    // tcph.acknowledge = htonl(1);
    // tcph.offset = (htons(5) >> 12);
    // tcph.reserved = 0;
    // tcph.urg = 0;
    // tcph.ack = 0;
    // tcph.psh = 0;
    // tcph.rst = 0;
    // tcph.syn = 0;
    // tcph.fin = 1;
    // tcph.window = htons(512);

    psdh.source = iph.source;
    psdh.dest = iph.dest;
    psdh.zero = 0;
    psdh.protocol = iph.protocol;
    psdh.seglen = tcph.offset*4;
    tcph.urgent = 0;

    tmph = (uint16_t*)malloc(2*(psdh_len + tcph_len));
    memcpy(tmph, &psdh, psdh_len);
    memcpy(tmph+psdh_len, &tcph, tcph_len);
    tcph.checksum = 0;
    tcph.checksum = checksum(tmph, psdh_len + tcph_len);
    free(tmph);

    
    buffer = (uint8_t*)malloc(iph_len + tcph_len);
    memcpy(buffer, &iph, iph_len);
    memcpy(buffer+iph_len, &tcph, tcph_len);
    
    if(sendto(server_sd, buffer, iph_len+tcph_len, 0, (struct sockaddr*) &client_addr, addr_len) < 0){
        perror("Error: send messages\n");
    }
    free(buffer);
    printf("Success: send segment from server to client\n");

    close(server_sd);
    return 0;
}
