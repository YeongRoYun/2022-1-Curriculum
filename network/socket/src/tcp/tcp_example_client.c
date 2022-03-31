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
	char buf[1000];
	unsigned int data_len;

	int err;

	unsigned int addr;
	unsigned short port;

	if(argc != 3) {
		printf("Usage : %s server_ip server_port\n", argv[0]);
		return 0;
	}

	addr = inet_addr(argv[1]);
	port = htons(atoi(argv[2]));

	memset((char *)&server_addr, 0, sizeof(server_addr));
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if(sockfd < 0) {
		printf("Creating a socket failed\n");
		return 0;
	}

	// Set up server_addr (struct sockaddr_in)
	server_addr.sin_addr.s_addr = addr;
	server_addr.sin_port = port;
	server_addr.sin_family = AF_INET;
	
	err = connect(sockfd, (struct sockaddr*) &server_addr, sizeof(server_addr));// Send a connection request to the server using Socket API

	while(1) {
		strcpy(buf, "2018320135");
		data_len = send(sockfd, buf, strlen(buf) + 1, 0);// Send your Student ID to the server using Socket API
		// Ex - Data - "2020010640", Data len - strlen("2020010640")+1
		if(data_len < 0) {
			printf("Send Error\n");
			break;
		}
		
		data_len = recv(sockfd, buf, sizeof(buf), 0);// Receive a reply from the server
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

