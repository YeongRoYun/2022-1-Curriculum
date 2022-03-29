# Socket

### Why use?
> used to specify endhost

### Be careful
> IP address and Port must be network byte order(MSB, human readable)

### functions

1. Socket address
   1. Address Families
      1. AF_INET: Address Family for IPv4
      2. AF_INET6: Address Family for IPv6
      3. AF_UNIX: Address Family for Local
   2. base
      ```
      <sys/socket.h>
      struct sockaddr {
      __uint8_t       sa_len;         //IP address + Port length, not require on POSIX
      sa_family_t     sa_family;      //address family 
      char            sa_data[14];    //IP address + Port
      };                              
      ```
   3. internet
      ```
      <netinet/in.h>
      //Used with AF_INET                 
      struct sockaddr_in {            
      __uint8_t       sin_len;    //same as sa_len
      sa_family_t     sin_family; //AF_INET(IPv4)
      in_port_t       sin_port;   //2bytes, network byte order
      struct  in_addr sin_addr;   //4bytes, Ip address
      char            sin_zero[8];//dummy, all zeros
      };
      ```
    4. local
	```   
	//<sys/un.h>
	//Used when AF_UNIX
	struct  sockaddr_un {
		unsigned char   sun_len;        //the length of path
		sa_family_t     sun_family;     //AF_UNIX
		char            sun_path[104];  //Path Name(socket name)
	};
	```


2. Changing data for networking
    1. byte order
       ```
       <arpa/inet.h>
       short ntohs(short x);
       short htons(short x);
       long ntohs(long x);
       long htons(long x);
       ```
       - -s for port, -l for ip address
    2. ip address string to binary(NBO)
       ```
       <arapa/inet.h>
       int inet_pton(int af, const char *src, void *dst);
       ```
       - af: AF_INET or AF_INET6
       - src: Ip address
       - dst: For binary, network byte order
       - return: 1=true


3. Communication
   1. create socket descriptor
      ```
      <sys/socket.h>
      int socket(int domain, int type, int protocol);
      ```
      - domain: a communications domain within which communication will take place(Address Families)
        - AF_INET: Ipv4
        - AF_INET6: Ipv6
        - AF_UNIX: Local
      
      - type: Format of transfer
        - SOCK_STREAM: sequenced, reliable, two-way connection based byte streams
        - SOCK_DGRAM: datagrams (connectionless, unreliable messages of a fixed (typically small) maximum length)
        - SOCK_RAW: Access to internal network protocols and interfaces
      
      - (ip) protocol: protocols in domain
        - specifies a particular protocol to be used with the socket
        - 0 is automatic
        - AF_INET
          - IPPROTO_ICMP
          - IPPROTO_TCP
          - IPPROTO_UDP
        - return -1 is failure, else return socket descriptor
   
   2. Allocate address to socket
      ```
      <sys/socket.h>
      int bind(int s, const struct sockaddr* addr, int addrlen);
      ```
      - s: socket descriptor
      - addr: sockaddr allocated to s
      - addrlen: addrlen, usually sizeof(struct sockaddr)
      - return < 0 is falure, else return 0
      
   3. Create listen queue in server
      ```
      <sys/socket.h>
      int listen(int s, int backlog);
      ```
      - s: socket descriptor
      - backlog: sizeof(queue)
      - return < 0 is failure, else return 0
      - requests after the queue is full are discarded
      
   4. Accept a request of client in server
      ```
      <sys/socket.h>
      int accept(int s, struct sockaddr* addr, socketlen_t* addrlen);
      ```
      - s: socket descriptor
      - addr: filled with the client address if successing
      - addrlen: initially contain the amount of space pointed to by address, on return it will contain the actual length(in bytes) of the address returned
      - return < 0 is failure, else return sock descriptor for communicating a client
   
   5. Connect with server
      ```
      <sys/socket.h>
      int connect(int s, struct sockaddr* addr, int addrlen);
      ```
      - s: socket descriptor
      - addr: server address information
      - addrlen: sizeof(addr)
      - return < 0 is failure
   
   6. Send/Receive messages
      1. Necessary connecting hosts
      
	 ```
	 <unistd.h>
	 size_t read(int fd, void* buf, size_t nbytes);
	 size_t write(int fd, const void* buf, size_t nbytes);
	 ```
	 - fd: file/socket descriptor
	 - buf: read/write buffer
	 - nbytes: sizeof(buffer)
	 - return < 0 is failure, else return the size of read/write bytes
	 
	 ```
	 <sys/socket.h>
	 int recv(int s, void* buf, size_t nbytes, int flags);
	 int send(int s, const void* buf, size_t nbytes, int flags);
	 ```
	 - s: socket descriptor
	 - buf: recv/send buffer
	 - nbytes: sizeof(buffer)
	 - flags
	   - MSG_OOB
	     - process out-of-band data
	     - 
           - MSG_PEEK
             - peek at incoming message
           - MSG_WAITALL
             - wait for full request or error
         - return < 0 is failure, else return the size of recv/send bytes
    
       2. Unnecessary connecting hosts
       
          ```
	  <sys/socket.h>
	  int recvfrom(int s, void* buf, size_t nbytes, int flags, const struct sockaddr* src_addr, socklen_t* addrlen);
	  int sendto(int s, const void* buf, size_t nbytes, int flags, const struct sockaddr* dest_addr, socklen_t addrlen);
	  ```
	  - src_addr: communication isn't connection-oriented and src_addr isn't null, on return, it filled with src_addr
	  - addrlen in recvfrom is passed initially as sizeof(src_addr). on return, it filled with actual sizeof(src_addr)
	  - dest_addr: must specify dest_addr
	  - return < 0 is failure, else return the size of recvfrom/sendto bytes

   7. Close socket
      ```
      <unistd.h>
      int close(int fd);
      ```
      - return < 0 is failure, else return 0

4. Mechanism
   1. TCP
      - socket(AF, SOCK_STREAM, protocol)
      - Server: socket() -> bind() -> listen() -> accept() -> read/write() -> close()
      - Client: socket() -> [bind()] -> connect() -> read/write() -> close()
      - After accepting, client_sd is returned for communicating with client
      - Client's bind is optional
   
   2. UDP
      - socket(AF, SOCK_DGRAM, protocol)
      - Server: socket() -> [bind()] -> read/write() -> close()
      - Client: socket() -> [bind()] -> read/write() -> close()
      - First reciever should be bound
      - when recieving msg, sender's address is passed together and automatically bound
   
   3. RAW
      - socket(AF, SOCK_RAW, protocol)
      - user handles protocol headers manually
      -  generated by root user only

5. Raw socket communication
   1. create socket descriptor
      '''
      <sys/socket.h>
      int socket(domain, AF_RAW, protocol);
      '''
      - protocol
	```
	<netinet/in.h>
	```
	
        * reference(https://www.ibm.com/docs/en/i/7.4?topic=ssw_ibm_i_74/apis/socket.htm#HDRSCKUS)
	* protocol number can be any except for IPPROTO_TCP and IPPROTO_UDP
	* Each raw socket is associated with one IP protocol number, and receives all data for that protocol.
	* 0 (IPPROTO_IP) is specified, IP sends all data received from all the protocol numbers except IPPROTO_TCP and IPPROTO_UDP.
	* 255 (IPPROTO_RAW) is specified, a user must ensure that the IP header data is included in th data sent out on an output operation 
   
   
   2. 
