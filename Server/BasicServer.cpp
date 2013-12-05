#include "BasicServer.h"

void BasicServer::initialize(int portNo) {
	// define a new socket for internet domain & streaming
	// 0 is a magic number
	this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
	// test the initialisation
	if (this->sockfd < 0) 
		error("ERROR opening socket");
	// initialise the buffer. 'cause security
	bzero((char *) &this->serv_addr, sizeof(this->serv_addr));
	// get the port number. Trust the user for the input
	this->portno = portNo;
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
	printf("%i, %i\n", this->sockfd, this->clilen);
}
	
void BasicServer::connectServer() {
	printf("%i, %i\n", this->sockfd, this->clilen);
	this->newsockfd = accept(this->sockfd, 
			(struct sockaddr *) &this->cli_addr, 
			&this->clilen);
	if (this->newsockfd < 0) 
		error("ERROR on accept");
}

void BasicServer::writeMessage(char * msg) {
	int tmp;
	tmp = write(this->newsockfd, msg, 18);
	if (tmp < 0) error("ERROR writing to socket");
}

void BasicServer::readMessage() {
	char buffer[256];
	int tmp;
	bzero(buffer,256);
	tmp = read(this->newsockfd, buffer, 255);
	if (tmp < 0) error("ERROR reading from socket");
	// read loudly
	printf("Here is the message: %s\n",buffer);	
}

void BasicServer::process() {
	while(1) {
		writeMessage((char *) "I've got your message");
		// initialize the buffer and fill it	
		readMessage();
		// answer
		writeMessage((char *) "I've got your message");
	}
}

void *BasicServer::run() {
	int pid;
	while(1) {
		// listen to the socket
		this->connectServer();
		pid = fork();

		if (pid == 0) {
			close(this->sockfd);
			process();
			exit(0);
		} else {
	//		close(this->newsockfd);
		}
	}
}

static void *BasicServer::runWrapper(void *context) {
	return ((BasicServer *) context)->run();
}

void BasicServer::serverMain(int portNo) {
	
	initialize(portNo);
	// while true listen. Good student		  	
	pthread_t t;
	// make a copy of the object to keep the 
	// field initialized
	BasicServer tmp(*this);
	printf("%i, %i\n", this->sockfd, this->clilen);
	printf("%i, %i\n", tmp.sockfd, tmp.clilen);
	pthread_create(&t, NULL, &BasicServer::runWrapper, &tmp);
	// Crappy instruction : it will be a dead lock !
	// wait for the thread to finish
//	pthread_join(&t, NULL);
	// close the door
	close(this->newsockfd);
	close(this->sockfd);
}
