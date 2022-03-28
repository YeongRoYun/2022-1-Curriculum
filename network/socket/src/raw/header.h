#ifndef __HEADER_H__

#define __HEADER_H__

#include<stdint.h>
#include<stdlib.h>
#include<stdio.h>

//RFC 791
struct ip_header {
    uint8_t version:         4;
    uint8_t ihl:             4;
    uint8_t tos:             8;
    uint16_t totlen:        16;

    uint16_t id:            16;
    uint8_t flags:           3;
    uint16_t offset:        13;

    uint8_t ttl:             8;
    uint8_t protocol:        8;
    uint16_t checksum:      16;

    uint32_t source:        32;
    uint32_t dest:          32;
};

//RFC 793
struct tcp_header {
    uint16_t source:        16;
    uint16_t dest:          16;
    uint32_t sequence:      32;
    uint32_t acknowledge:   32;
    uint8_t offset:          4;
    uint16_t reserved:       4;
    uint8_t urg:             1;
    uint8_t ack:             1;
    uint8_t psh:             1;
    uint8_t rst:             1;
    uint8_t syn:             1;
    uint8_t fin:             1;
    uint16_t window:        16;
    uint16_t checksum:      16;
    uint16_t urgent:        16;
};

//RFC 768
struct udp_header {
    uint16_t source:        16;
    uint16_t dest:          16;

    uint16_t len:           16;
    uint16_t checksum:      16;
};

//RFC 793
struct pseudo_header {
    uint32_t source:        32; 
    uint32_t dest:          32; 
    uint8_t zero:            8;
    uint8_t protocol:        8; 
    uint16_t seglen:        16;
};


//RFC 1071
uint16_t checksum(uint16_t* addr, size_t count) {
    register uint32_t sum = 0;
    while(count > 1) {
        sum += *addr++;
        count -= 2;
    }
    if(count > 0) {
        sum += *((unsigned char*)addr);
    }
    while(sum >> 16) {
        sum = (sum & 0xffff) + (sum >> 16);
    }
    return (uint16_t)~sum;
}


#endif