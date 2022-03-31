#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>

int main(int argc, char * argv[]) {
	int sockfd;
	struct sockaddr_in server_addr;
	unsigned int data_len;

	unsigned int addr;

	if(argc != 2) {
		printf("Usage : %s peer_ip\n", argv[0]);
		return 0;
	}

	addr = inet_addr(argv[1]);

	memset((char *)&server_addr, 0, sizeof(server_addr));
	
	sockfd = // Create socket

	if(sockfd < 0) {
		printf("Creating a socket failed\n");
		return 0;
	}

	// Set up server_addr (struct sockaddr_in)
	// (fill the sin_family and s_addr. sin_port is not required)

	
	data_len = // Send your Student ID to the server using Socket API
	if(data_len < 0) {
		printf("Send Error\n");
		break;
	}

	close(sockfd);
}

