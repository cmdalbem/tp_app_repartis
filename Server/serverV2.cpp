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

void usage(int argc) 
{
	if (argc < 2) {
		fprintf(stderr,"ERROR, no port provided\n");
		exit(1);
	}
}

class Server {
	// defines two file descriptor and the port number
	int sockfd, newsockfd, portno;
	// size of the client
	socklen_t clilen;
	// internet address of the server and the client
	struct sockaddr_in serv_addr, cli_addr;

	public:

	void initialize(char *argv[]) {
		// define a new socket for internet domain & streaming
		// 0 is a magic number
		this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
		// test the initialisation
		if (this->sockfd < 0) 
			error("ERROR opening socket");
		// initialise the buffer. 'cause security
		bzero((char *) &this->serv_addr, sizeof(this->serv_addr));
		// get the port number. Trust the user for the input
		this->portno = atoi(argv[1]);
		// define the server adress
		// address family : INET
		this->serv_addr.sin_family = AF_INET;
		// server -> the host is the machine
		this->serv_addr.sin_addr.s_addr = INADDR_ANY;
		// translate the port number in network byte order
		this->serv_addr.sin_port = htons(this->portno);
		// bind the socket to the server
		if (bind(this->sockfd, (struct sockaddr *) &this->serv_addr,
					sizeof(this->serv_addr)) < 0) 
			error("ERROR on binding");

		listen(this->sockfd, 5);
		// wait until a client connects to the server
		this->clilen = sizeof(this->cli_addr);
	}

	void connectServer() {
		this->newsockfd = accept(this->sockfd, 
				(struct sockaddr *) &this->cli_addr, 
				&this->clilen);
		if (this->newsockfd < 0) 
			error("ERROR on accept");
	}

	void writeMessage(char * msg) {
		int tmp;
		tmp = write(this->newsockfd, msg, 18);
		if (tmp < 0) error("ERROR writing to socket");
	}

	void readMessage() {
		char buffer[256];
		int tmp;
		bzero(buffer,256);
		tmp = read(this->newsockfd, buffer, 255);
		if (tmp < 0) error("ERROR reading from socket");
		// read loudly
		printf("Here is the message: %s\n",buffer);	
	}

	void process() {
		while(1) {
			// initialize the buffer and fill it	
			readMessage();
			// answer
			writeMessage("I've got your message");
		}
	}

	void serverMain(int argc, char *argv[]) {
		int pid;

		// shout at lazy user
		usage(argc);

		initialize(argv);
		// while true listen. Good student		  	
		while(1) {
			// listen to the socket
			this->connectServer();
			pid = fork();

			if (pid == 0) {
				close(this->sockfd);
				process();
				exit(0);
			} else {
				close(this->newsockfd);
			}
		}
		// close the door
		close(this->newsockfd);
		close(this->sockfd);
	}

};




int main(int argc, char *argv[])
{
	Server server;
	server.serverMain(argc, argv);

	return 0; 
}
