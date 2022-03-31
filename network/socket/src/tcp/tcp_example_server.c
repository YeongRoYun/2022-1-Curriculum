#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>

int main(int argc, char * argv[]) {
	int sockfd, connfd;
	struct sockaddr_in my_addr;
	struct sockaddr_in peer_addr;
	unsigned int peer_addr_len;

	char buf[1000];
	unsigned int data_len;
	int err;

	unsigned int addr;
	unsigned short port;

	if(argc != 3) {
		printf("Usage : %s my_ip my_port\n", argv[0]);
		return 0;
	}

	addr = inet_addr(argv[1]);
	port = htons(atoi(argv[2]));

	peer_addr_len = sizeof(peer_addr);
	memset((char *)&my_addr, 0, sizeof(my_addr));
	
	
	sockfd = // Create socket

	if(sockfd < 0) {
		printf("Creating a socket failed\n");
		return 0;
	}
	// Set up server_addr (struct sockaddr_in)

	
	err = // Bind (Local IP, Local port) to the socket
	if(err < 0) {
		printf("Bind Error\n");
		close(sockfd);

		return 0;
	}
	
	err = // Make the socket a listening socket.
	if(err < 0) {
		printf("Listen Error\n");
		close(sockfd);

		return 0;
	}

	while(1) {
		connfd = // Accept a connection request from a client
		if(connfd < 0) {
			printf("Accept Error\n");
			break;
		}
		printf("Connection Request from IP : %s, Port : %d has been accepted\n", inet_ntoa(peer_addr.sin_addr), ntohs(peer_addr.sin_port));
		
		data_len = // Receive a data from a client
		if(data_len < 0) {
			printf("Receive Error\n");
			break;
		}
		printf("Length : %d, Receiving : %s, ", data_len, buf);
		printf("from IP : %s, Port : %d\n", inet_ntoa(peer_addr.sin_addr), ntohs(peer_addr.sin_port));
		
		data_len = // Reply to the client using Socket API
		// Ex - Data - buf, Data len - data_len
		if(data_len < 0) {
			printf("Send Error\n");
			break;
		}
		close(connfd);
	}
	close(sockfd);
}

