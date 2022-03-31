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

	unsigned int addr;
	unsigned short port;

	int err;

	if(argc != 3) {
		printf("Usage : %s my_ip my_port\n", argv[0]);
		return 0;
	}

	addr = inet_addr(argv[1]);
	port = htons(atoi(argv[2]));

	peer_addr_len = sizeof(peer_addr);

	memset((char *)&my_addr, 0, sizeof(my_addr));
	
	
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);// Create socket

	if(sockfd < 0) {
		printf("Creating a socket failed\n");
		return 0;
	}
	// Set up server_addr (struct sockaddr_in)
	my_addr.sin_addr.s_addr = addr;
	my_addr.sin_port = port;
	my_addr.sin_family = AF_INET;

	err = bind(sockfd, (struct sockaddr*) &my_addr, sizeof(my_addr));// Bind (Local IP, Local port) to the socket
	if(err < 0) {
		printf("Bind Error\n");
		close(sockfd);

		return 0;
	}

	while(1) {
		
		data_len = recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr*) &peer_addr, &peer_addr_len);// Receive a data from a client
		// ?? (??, buf, 1000, 0, ??, ??)
		if(data_len < 0) {
			printf("Receiving Error\n");
			break;
		}
		printf("Length : %d, Receiving : %s ", data_len, buf);
	        printf("From IP : %s, Port : %d\n", inet_ntoa(peer_addr.sin_addr), ntohs(peer_addr.sin_port));

		data_len = sendto(sockfd, buf, sizeof(buf), 0, (struct sockaddr*) &peer_addr, peer_addr_len);// Reply to the client using Socket API
		// ?? (??, buf, data_len, 0, ??, ??)
		if(data_len < 0) {
			printf("Send Error\n");
			break;
		}
	}
	close(sockfd);
	return 0;
}

