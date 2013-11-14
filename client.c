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

int main(int argc, char *argv[])
{
	// defines the file descriptor of the socket
    int sockfd;
	// defines the port number 
	int portno;
	// will be use to count the length of the message
	int n;
	// buffer used for the communication
	char buffer[256];
	// internet address of the server
    struct sockaddr_in serv_addr;
	// pointer to the computer hosting the server
    struct hostent *server;

    // shout at the bad user
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
	
	// be confident that your user is not a troll
    portno = atoi(argv[2]);
	// create a socket 	AF_INET for Internet Domain
	// 					SOCK_STREAM for a stream
	//					0 is a magic number
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	// test the opening
    if (sockfd == -1) 
        error("ERROR opening socket");
	// get the host. Once again, be confident in the input
    server = gethostbyname(argv[1]);
	// test the initialisation
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
	
	// initialize serv_addr
    bzero((char *) &serv_addr, sizeof(serv_addr));
	// address family -> internet
    serv_addr.sin_family = AF_INET;
	// copy the host address
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
	// translate the port into web byte order
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
			error("ERROR connecting");
		
	// it's gonna be a long talk
	while(1) {
		printf("Please enter the message: ");
		// initialize & read into the buffer
		bzero(buffer,256);
		fgets(buffer,255,stdin);
		// write to the other computer
		n = write(sockfd,buffer,strlen(buffer));
		if (n < 0) 
			error("ERROR writing to socket");
		// wait for the answer
		bzero(buffer,256);
		n = read(sockfd,buffer,255);
		if (n < 0) 
			error("ERROR reading from socket");
		printf("%s\n",buffer);
	}
	printf("While true failed");
	// close the door
    close(sockfd);
    return 0;
}
