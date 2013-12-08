#include "Receiver.h"
#include "Connector.h"
#include "Server.h"

using namespace std;

// initialize the server
void Receiver::initialize(int portNo) {
	// define a new socket for internet domain & streaming
	// 0 is a magic number
	this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
	// test the initialisation
	if (this->sockfd < 0) 
		error("ERROR opening socket");
	int yes=1;
	//char yes='1'; // use this under Solaris

	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
		perror("setsockopt");
		exit(1);
	}
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
void Receiver::connectServer() {
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
void Receiver::writeMessage(char * msg) {
	int tmp;
	tmp = write(this->newsockfd, msg, strlen(msg));
	if (tmp < 0) error("ERROR writing to socket");
}

// answer a client
// buffer need to be more thant 255 chars
void Receiver::readMessage(char *buffer) {
	int tmp;
	bzero(buffer,256);
	tmp = read(this->newsockfd, buffer, 255);
	if (tmp < 0) error("ERROR reading from socket");
}

// defines the behaviour of the server 
void Receiver::process() {
	char buffer[256];
	while(1) {
		cout << "Waiting for a message" << endl;
		readMessage(buffer);
		this->server->handleMessage(buffer);
	}
}
void *Receiver::processWrapper(void *context) {
	((Receiver *) context)->process();
	return (void *) 0;
}

// everything needed to run a server 
void *Receiver::run() {
	initialize(this->portNo);
	//int pid;
	// while true listen. Good student		  	
	while(1) {
		// listen to the socket
		cout<<"Waiting for a connection"<<endl;
		this->connectServer();
		// new thread to accept other connections	
		pthread_t t;
		Receiver *threadedReceiver = new Receiver(*this);
		pthread_create(&t, NULL, &Receiver::processWrapper, threadedReceiver);
	}
	close(this->sockfd);
	close(this->newsockfd);
	return 0;
}

// wrapper to launch a thread
void *Receiver::runWrapper(void *context) {
	return ((Receiver *) context)->run();
}

// launch a server on port portNo	
void Receiver::serverMain(int machineId, int portNo) {
	this->machineId = machineId;
	this->portNo = portNo;

	pthread_t t;

	// make a copy of the object to keep the 
	// field initialized
	threadedServer = new Receiver(*this);
	pthread_create(&t, NULL, &Receiver::runWrapper, threadedServer);
}

// use to signal an error & quit the thread
void Receiver:: error(const char *msg)
{
	perror(msg);
	pthread_exit(NULL);
}
