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
      
      - protocol: protocols in domain
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
      - addrlen: sizeof(struct sockaddr)
      - return < 0 is falure, else return 0
      
   3. Create listen queue in server
      ```
      <sys/socket.h>
      int listen(int s, int backlog);
      ```
      - s: socket descriptor
      - backlog: sizeof(queue)
      - return < 0 is falure, else return 0
      - Requests after the queue is full are discarded
      
   4. Accept a request of client in server
      ```
      <sys/socket.h>
      int accept(int s, struct sockaddr* addr, socketlen_t* addrlen);
      ```
      


//<sys/socket.h>
//Accept a client and return descriptor for communication
int accept(int s, struct sockaddr *addr, socklen_t *addrlen);
//s: socket descriptor
//addr: client address info. pass empty addr and get it
// addrlen: point of a sockaddr size(len is written)
//return: connected socket descriptor, use this to communicate a client!

////////////////////////////////////////////////////////

////////////////////////////////////////////////////////
//For client
//<sys/socket.h>
//Connect server
int connect(int s, struct sockaddr* serv_addr, int addrlen);
//s: client socket descriptor
//serve_addr: server addr info. pass perfect info
//addrlen: size of sockaddr
//return: 0=true
////////////////////////////////////////////////////////


////////////////////////////////////////////////////////
//For both
//<unistd.h>
int close(int fd);
//return: 0=true, -1=false

//<unistd.h>
size_t write(int fd, const void* msg, size_t nbytes);
size_t read(int fd, void* buf, size_t nbytes);
//fd: sender/receiver descriptor
//return: the size of bytes = success/ -1 = fail


//<sys/socket.h>
//Send/Receive messages in TCP mainly
int send(int s, const void* msg, size_t len, int flags);
int recv(int s, void* buf, size_t len, int flags);
//s: sender/receiver socket descriptor
//msg: messages
//len: the size of messages
//flags: option
//return: the size of bytes = success/ -1 = fail

//<sys/socket.h>
//Send/Receive messages in UDP mainly
int sendto(int s, const void* msg, size_t len, int flags, const struct sockaddr* dest_addr, socklen_t addrlen);
int recvfrom(int s, const void* buf, size_t len, int flags, const struct sockaddr* src_addr, socklen_t* addrlen);
//dest_addr: destination address
//len of sendto is read
//src_addr: source address
//len of recvfrom is written
//return: the size of bytes = success/ -1 = fail

//     MSG_DONTWAIT     ||    MSG_NOSIGNAL
//if waiting, return -1 || if disconnected, don't receive SIGPIPE

////////////////////////////////////////////////////////


////////////////////////////////////////////////////////
//TCP communications
//server: socket() -> bind() -> listen() -> accept() -> read/write() -> close()
//client: socket() -> connect() -> read/write() -> close()

//UDP communications
//server: socket() -> bind() -> read/write() -> close()
//client: socket() -> bind() -> read/write() -> close()
//First receiver must be bound its address!
////////////////////////////////////////////////////////

////////////////////////////////////////////////////////
//Raw Socket
//Manage TCP/IP headers manually. treat as user data
//Features
////1. Can accessed at network, transport, and applicatoin layers
////2. Can handle network and transport header
////3. Have a harzard since all packets are recieved
////4. Port doesn't exist
//Examples - Wireshark

//socket
s = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP/IPPROTO_TCP/IPPROTO_UDP);
//protocol can be automatic!
//protocols are defined in <netinet/in.h>
//Created by root user only(tcp/ip suite is in kernel!)

//<sys/socket.h>
//get/set socket options
int getsockopt(int  s, int level, int optname, void *optval, socklen_t *optlen);
int setsockopt(int s, int  level,  int  optname,  const  void  *optval, socklen_t optlen);
//s: socket
//level: the level of socket(layer depth)
//optname: the number of socket option
//optval: opt buffer
//optlen: opt buffer size
////////////////////////////////////////////////////////
