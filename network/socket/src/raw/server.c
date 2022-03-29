#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<stdio.h>
#include<netinet/tcp.h>
#include<netinet/ip.h>

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
    struct tcphdr* tcphdr;
    struct ip* iphdr;
    struct pseuhdr* pseuhdr;
    uint16_t* tmph;
    uint8_t* packet = (uint8_t*)malloc(40);


    if((server_sd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0) {
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
    
    memset(packet, 0, 40);
    
    iphdr = (struct ip*) packet;

    iphdr->ip_v = 4;
    iphdr->ip_hl = 5;
    iphdr->ip_tos = 0;
    iphdr->ip_len = 40;
    iphdr->ip_id = 0;
    iphdr->ip_off = 0;
    iphdr->ip_ttl = 152;
    iphdr->ip_p = IPPROTO_TCP;
    iphdr->ip_src = server_addr.sin_addr;
    iphdr->ip_dst = client_addr.sin_addr;
    iphdr->ip_sum = checksum((uint16_t*) &iphdr, 20);

    tcphdr = (struct tcphdr*) (packet + 20);

    tcphdr->th_sport = server_addr.sin_port;
    tcphdr->th_dport = client_addr.sin_port;
    tcphdr->th_seq = 0;
    tcphdr->th_ack = 1;
    tcphdr->th_off = 5;
    tcphdr->th_flags = TH_FIN;
    tcphdr->th_win = htons(512);
    tcphdr->th_urp = 0;
    
    pseuhdr = (struct pseuhdr*)malloc(sizeof(struct pseuhdr));
    pseuhdr->source = iphdr->ip_src.s_addr;
    pseuhdr->dest = iphdr->ip_dst.s_addr;
    pseuhdr->zero = 0;
    pseuhdr->protocol = iphdr->ip_p;
    pseuhdr->seglen = 20;

    tmph = (uint16_t*)malloc(2*(sizeof(pseuhdr) + 20));
    memset(tmph, 0, 2*(sizeof(pseuhdr)+20));
    memcpy(tmph, &pseuhdr, sizeof(pseuhdr));
    memcpy(tmph+sizeof(pseuhdr), &tcphdr, 20);
    tcphdr->th_sum = checksum(tmph, sizeof(pseuhdr) + 20);
    free(tmph);
    
    if(sendto(server_sd, packet, 40, 0, (struct sockaddr*) &client_addr, addr_len) < 0){
        perror("Error: send messages\n");
    }
    free(packet);
    printf("Success: send segment from server to client\n");

    close(server_sd);
    return 0;
}
