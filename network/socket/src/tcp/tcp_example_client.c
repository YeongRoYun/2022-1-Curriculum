#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
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
	
	
	err = // Send a connection request to the server using Socket API

	while(1) {
		
		data_len = // Send your Student ID to the server using Socket API
		// Ex - Data - "2020010640", Data len - strlen("2020010640")+1
		if(data_len < 0) {
			printf("Send Error\n");
			break;
		}
		
		data_len = // Receive a reply from the server
		if(data_len < 0) {
			printf("Receive Error\n");
			break;
		}
		printf("Receiving : %s\n", buf);
	}
	close(sockfd);
}

