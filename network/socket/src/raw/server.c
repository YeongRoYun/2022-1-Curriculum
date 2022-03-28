#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<stdio.h>

#include"header.h"

#define SERVER_IP "192.168.0.3"
#define SERVER_PORT 4000
#define CLIENT_IP "192.168.0.3"
#define CLIENT_PORT 3000
#define SIZE 200

int main(int argc, char** argv){

    int sd;
    int on = 1;
    
    struct sockaddr_in client_addr, server_addr;
    struct tcp_header tcph;
    struct ip_header iph;
    struct pseudo_header psdh;
    uint16_t* tmph;
    uint8_t* buffer;
    
    size_t sent_len;
    size_t iph_len = sizeof(struct ip_header);
    size_t tcph_len = sizeof(struct tcp_header);
    size_t psdh_len = sizeof(struct pseudo_header);
    socklen_t addr_len = sizeof(struct sockaddr);

    memset(&client_addr, 0, addr_len);
    memset(&server_addr, 0, addr_len);
    memset(&iph, 0, iph_len);
    memset(&tcph, 0, tcph_len);
    memset(&psdh, 0, psdh_len);

    if((sd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP)) < 0) {
        perror("Error: socket\n");
        exit(1);
    }

    if(setsockopt(sd, IPPROTO_IP, IP_HDRINCL, &on, sizeof(on)) < 0) {
		printf("set socket option error\n");
        exit(1);
	}

    inet_pton(AF_INET, CLIENT_IP, &client_addr.sin_addr);
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(CLIENT_PORT);
    
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);

    if(bind(sd, (struct sockaddr*) &server_addr, addr_len) < 0){
        perror("Error: bind\n");
        exit(1);
    }
    
    iph.version = 4;
    iph.ihl = 5;
    iph.tos = 0;
    iph.totlen = iph_len + tcph_len;
    iph.id = 0;
    iph.flags = 0x2;
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
    tcph.window = 512;

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

    
    while((sent_len = sendto(sd, buffer, iph_len+tcph_len, 0, (struct sockaddr*) &client_addr, addr_len)) >= 0);
    if(sent_len < 0) {
        perror("Error: send\n");
        exit(1);
    }
    
    free(buffer);
    if(close(sd) < 0) {
        perror("Error: exit socket at server");
        exit(1);
    }
    return 0;
}