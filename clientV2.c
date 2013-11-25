#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

void usage(int argc, char *argv[]) {
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
}


typedef struct {
	// defines the file descriptor of the socket
    int sockfd;
	// defines the port number 
	int portNo;
	// internet address of the server
    struct sockaddr_in serv_addr;
	// pointer to the computer hosting the server
    struct hostent *server;

} client;

void setPortNo(client *c, int portNo) {
	c->portNo = portNo;
}

void createSocket(client *c, char * hostName) {
	// create a socket 	AF_INET for Internet Domain
	// 					SOCK_STREAM for a stream
	//					0 is a magic number
    c->sockfd = socket(AF_INET, SOCK_STREAM, 0);
	// test the opening
    if (c->sockfd == -1) 
        error("ERROR opening socket");
	// get the host. Once again, be confident in the input
    c->server = gethostbyname(hostName);
	// test the initialisation
    if (c->server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
	// initialize serv_addr
    bzero((char *) &c->serv_addr, sizeof(c->serv_addr));
	// address family -> internet
    c->serv_addr.sin_family = AF_INET;
	// copy the host address
    bcopy((char *)c->server->h_addr, 
         (char *)&c->serv_addr.sin_addr.s_addr,
         c->server->h_length);
	// translate the port into web byte order
    c->serv_addr.sin_port = htons(c->portNo);
}	

void connectSocket(client *c) {
	if (connect(c->sockfd,(struct sockaddr *) &c->serv_addr,sizeof(c->serv_addr)) < 0) 
				error("ERROR connecting");
}

void closeClient(client *c) {
	close(c->sockfd);
}

void writeMessage(client *c, char *buffer) {
	int tmp;
	tmp = write(c->sockfd, buffer, strlen(buffer));
	if (tmp < 0) 
			error("ERROR writing to socket");
}

void readMessage(client *c, char* buffer) {
	int tmp;
	bzero(buffer,256);
	tmp = read(c->sockfd, buffer, 255);
	if (tmp < 0) 
			error("ERROR reading from socket");
	printf("%s\n", buffer);		
}

int clientMain(int argc, char *argv[]) {
	// Captain obvious likes to describe the declaration of the client
	client c;
	// buffer used for the communication
	char buffer[256];
	
    // shout at the bad user
	usage(argc, argv);
	
	// be confident that your user is not a troll
    setPortNo(&c, atoi(argv[2]));
	createSocket(&c, argv[1]);
	
	// it's gonna be a long talk
	connectSocket(&c);		
		
	while(1) {
		printf("Please enter the message: ");
		// initialize & read into the buffer
		bzero(buffer,256);
		fgets(buffer,255,stdin);
		// write to the other computer
		writeMessage(&c, buffer);
		// wait for the answer
		readMessage(&c, buffer);
	}
	// close the door
	closeClient(&c);
}

int main(int argc, char *argv[])
{
    clientMain(argc, argv);

    return 0;
}
