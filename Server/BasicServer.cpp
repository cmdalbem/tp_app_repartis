#include "BasicServer.h"

// initialize the server
void BasicServer::initialize(int portNo) {
	// define a new socket for internet domain & streaming
	// 0 is a magic number
	this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
	// test the initialisation
	if (this->sockfd < 0) 
		error("ERROR opening socket");
	// initialise the buffer. 'cause security
	bzero((char *) &this->serv_addr, sizeof(this->serv_addr));
	// set the port number 
	this->portNo = portNo;
	// define the server adress
	// address family : INET
	this->serv_addr.sin_family = AF_INET;
	// server -> the host is the machine
	this->serv_addr.sin_addr.s_addr = INADDR_ANY;
	// translate the port number in network byte order
	this->serv_addr.sin_port = htons(this->portNo);
	// bind the socket to the server
	if (bind(this->sockfd, (struct sockaddr *) &this->serv_addr,
				sizeof(this->serv_addr)) < 0) 
		error("ERROR on binding");
	// listen on socket
	listen(this->sockfd, 5);
	this->clilen = sizeof(this->cli_addr);
}
	
// try to accept incoming connection 
void BasicServer::connectServer() {
	// wait until a client connects to the server
	this->newsockfd = accept(this->sockfd, 
			(struct sockaddr *) &this->cli_addr, 
			&this->clilen);
	// test the connexion
	if (this->newsockfd < 0) { 
		printf("Server ERROR on accept\n");
	}
}

// write a message to the client
void BasicServer::writeMessage(char * msg) {
	int tmp;
	tmp = write(this->newsockfd, msg, strlen(msg));
	if (tmp < 0) error("ERROR writing to socket");
}

// answer a client
void BasicServer::readMessage() {
	char buffer[256];
	int tmp;
	bzero(buffer,256);
	tmp = read(this->newsockfd, buffer, 255);
	if (tmp < 0) error("ERROR reading from socket");
	// read loudly
	printf("Server: %s\n",buffer);	
}

// defines the behaviour of the server 
void BasicServer::process() {
	char buffer[256];
	while(1) {
		// initialize the buffer and fill it	
		readMessage();
		// answer
		for(int i = 0; i < 255; i++) {
			buffer[i] = "X -> Y: I've got your message"[i];
		}
		buffer[0] = machineId + '0';
		buffer[5] = '?'; 
		writeMessage(buffer);
	}
}

// everything needed to run a server 
void *BasicServer::run() {
	initialize(this->portNo);
	int pid;
	// while true listen. Good student		  	
	while(1) {
		// listen to the socket
		this->connectServer();
		// fork to accept other connection	
		pid = fork();
		if (pid == 0) {
			// the child must behave correctly
			close(this->sockfd);
			process();
			exit(0);
		} else {
			close(this->newsockfd);
		}
	}
	close(this->sockfd);
	close(this->newsockfd);
}

// wrapper to launch a thread
void *BasicServer::runWrapper(void *context) {
	return ((BasicServer *) context)->run();
}

// launch a server on port portNo	
void BasicServer::serverMain(int machineId, int portNo) {
	this->machineId = machineId;
	this->portNo = portNo;

	pthread_t t;

	// make a copy of the object to keep the 
	// field initialized
	threadedServer = new BasicServer(*this);
	pthread_create(&t, NULL, &BasicServer::runWrapper, threadedServer);
}

// use to signal an error & quit the thread
void BasicServer:: error(const char *msg)
{
	perror(msg);
	pthread_exit(NULL);
}
