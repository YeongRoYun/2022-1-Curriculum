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
	socklen_t server_addr_len = sizeof(server_addr);
	char buf[1000];
	unsigned int data_len;

	unsigned int addr;
	unsigned short port;

	if(argc != 3) {
		printf("Usage : %s server_ip server_port\n", argv[0]);
		return 0;
	}

	addr = inet_addr(argv[1]);
	port = htons(atoi(argv[2]));

	memset((char *)&server_addr, 0, sizeof(server_addr));
	
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);// Create socket

	if(sockfd < 0) {
		printf("Creating a socket failed\n");
		return 0;
	}

	// Set up server_addr (struct sockaddr_in)
	server_addr.sin_addr.s_addr = addr;
	server_addr.sin_port = port;
	server_addr.sin_family = AF_INET;

	while(1) {
		strcpy(buf, "2018320135");
		data_len = sendto(sockfd, buf, strlen(buf) + 1, 0, (struct sockaddr*) &server_addr, server_addr_len);// Send your Student ID to the server using Socket API
		// Ex - Data - "2020010640", Data len - strlen("2020010640")+1
		if(data_len < 0) {
			printf("Send Error\n");
			break;
		}
		
		data_len = recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr*) &server_addr, &server_addr_len);// Receive a reply from the server
		if(data_len < 0) {
			printf("Receive Error\n");
			break;
		}

		printf("Receiving : %s\n", buf);
		break;
	}
	close(sockfd);
	return 0;
}

