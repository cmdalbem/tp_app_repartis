/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
	// defines two file descriptor and the port number
    int sockfd, newsockfd, portno;
	// size of the client
    socklen_t clilen;
	// buffer used for the communication
    char buffer[256];
	// internet address of the server and the client
    struct sockaddr_in serv_addr, cli_addr;
	// count for the length of the message
    int n;
	// shout at lazy user
    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }
	
	// define a new socket for internet domain & streaming
	// 0 is a magic number
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
	// test the initialisation
    if (sockfd < 0) 
        error("ERROR opening socket");
	// initialise the buffer. 'cause security
    bzero((char *) &serv_addr, sizeof(serv_addr));
	// get the port number. Trust the user for the input
    portno = atoi(argv[1]);
	// define the server adress
	// address family : INET
    serv_addr.sin_family = AF_INET;
	// server -> the host is the machine
    serv_addr.sin_addr.s_addr = INADDR_ANY;
	// translate the port number in network byte order
    serv_addr.sin_port = htons(portno);
	// bind the socket to the server
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
			sizeof(serv_addr)) < 0) 
             error("ERROR on binding");
	// while true listen. Good student		  
	
		// listen to the socket
		listen(sockfd, 5);
		// wait until a client connects to the server
		clilen = sizeof(cli_addr);
		newsockfd = accept(sockfd, 
					(struct sockaddr *) &cli_addr, 
					&clilen);
		if (newsockfd < 0) 
			error("ERROR on accept");
	while(1) {
		// initialize the buffer and fill it	
		bzero(buffer,256);
		n = read(newsockfd,buffer,255);
		if (n < 0) error("ERROR reading from socket");
		// read loudly
		printf("Here is the message: %s\n",buffer);
		// answer
		n = write(newsockfd,"I got your message",18);
		if (n < 0) error("ERROR writing to socket");
	}
	// close the door
    close(newsockfd);
    close(sockfd);
	
    return 0; 
}
