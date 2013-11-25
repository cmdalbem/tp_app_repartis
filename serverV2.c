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

void usage(int argc, char *argv[]) 
{
   if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }
}

typedef struct {
	// defines two file descriptor and the port number
    int sockfd, newsockfd, portno;
	// size of the client
    socklen_t clilen;
	// internet address of the server and the client
    struct sockaddr_in serv_addr, cli_addr;
} server;

void initialize(server *s, char *argv[]) {
	// define a new socket for internet domain & streaming
	// 0 is a magic number
    s->sockfd = socket(AF_INET, SOCK_STREAM, 0);
	// test the initialisation
    if (s->sockfd < 0) 
        error("ERROR opening socket");
	// initialise the buffer. 'cause security
    bzero((char *) &s->serv_addr, sizeof(s->serv_addr));
	// get the port number. Trust the user for the input
    s->portno = atoi(argv[1]);
	// define the server adress
	// address family : INET
    s->serv_addr.sin_family = AF_INET;
	// server -> the host is the machine
    s->serv_addr.sin_addr.s_addr = INADDR_ANY;
	// translate the port number in network byte order
    s->serv_addr.sin_port = htons(s->portno);
	// bind the socket to the server
    if (bind(s->sockfd, (struct sockaddr *) &s->serv_addr,
			sizeof(s->serv_addr)) < 0) 
        error("ERROR on binding");
		
	listen(s->sockfd, 5);
	// wait until a client connects to the server
	s->clilen = sizeof(s->cli_addr);
}

void connectServer(server *s) {
	s->newsockfd = accept(s->sockfd, 
				(struct sockaddr *) &s->cli_addr, 
					&s->clilen);
	if (s->newsockfd < 0) 
		error("ERROR on accept");
}

void writeMessage(server *s, char *msg) {
	char buffer[256];
	int tmp;
	tmp = write(s->newsockfd, msg, 18);
	if (tmp < 0) error("ERROR writing to socket");
}

void readMessage(server *s) {
	char buffer[256];
	int tmp;
	bzero(buffer,256);
	tmp = read(s->newsockfd, buffer, 255);
	if (tmp < 0) error("ERROR reading from socket");
	// read loudly
		printf("Here is the message: %s\n",buffer);	
}

void process(server *s) {
    while(1) {
	    // initialize the buffer and fill it	
    	readMessage(s);
	    // answer
    	writeMessage(s, "I've got your message");
    }
}

void serverMain() {
	server s;
    int pid;

	// shout at lazy user
 	usage(argc, argv);
	
	initialize(&s, argv);
	// while true listen. Good student		  	
	while(1) {
	    // listen to the socket
    	connectServer(&s);
        pid = fork();

        if (pid == 0) {
            close(s.sockfd);
            process(&s);
            exit(0);
        } else {
            close(s.newsockfd);
        }
	}
	// close the door
	close(s.newsockfd);
	close(s.sockfd);
}

int main(int argc, char *argv[])
{
    serverMain();

    return 0; 
}


