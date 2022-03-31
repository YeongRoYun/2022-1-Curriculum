#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include<arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char * argv[]) {
	int sockfd;
	struct sockaddr_in server_addr;
	unsigned int data_len;

	unsigned int addr;
	char buf[200];

	if(argc != 2) {
		printf("Usage : %s peer_ip\n", argv[0]);
		return 0;
	}

	addr = inet_addr(argv[1]);

	memset((char *)&server_addr, 0, sizeof(server_addr));
	
	sockfd = sock(AF_INET, SOCK_RAW, 200);// Create socket

	if(sockfd < 0) {
		printf("Creating a socket failed\n");
		return 0;
	}

	// Set up server_addr (struct sockaddr_in)
	// (fill the sin_family and s_addr. sin_port is not required)
	server_addr.sin_addr.s_addr = addr;
	
	strcpy(buf, "2018320135");
	data_len = sendto(sockfd, buf, strlen(buf) + 1, 0, (struct sockaddr*) &server_addr, sizeof(server_addr));// Send your Student ID to the server using Socket API
	if(data_len < 0) {
		printf("Send Error\n");
		close(sockfd);
		return 0;
	}

	close(sockfd);
	return 0;
}

