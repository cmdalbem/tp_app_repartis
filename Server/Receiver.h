#ifndef Receiver_H
#define Receiver_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <pubsub.h>
#include <pthread.h>

/* 
   A simple server in the internet domain using TCP
   The port number is passed as an argument 
*/
class Server;
class Receiver {
public:
	// launch a server on port portNo on a thread	
	
	Receiver() {}
	Receiver(Server *pserver){ this->server = pserver;}
		 
	void serverMain(int machineId, int portNo);
	// everything needed to run a server 
	void *run(); 

private:
	Server *server;
	// defines the behaviour of the server 
	static void *processWrapper(void *context);
	void process();
	// initialize the server
	void initialize(int portNo);
	// try to accept incoming connection 
	void connectServer(); 
	// write a message to the client
	void writeMessage(char * msg);
	// answer a client
	void readMessage(char *buffer);
	// wrapper to launch a thread
	static void *runWrapper(void *context);
	// use to signal an error & quit the thread
	void error(const char *msg);

	// id of the machine using this server
	int machineId;
	// defines two file descriptor and the port number
	int sockfd, newsockfd, portNo;
	// size of the client
	socklen_t clilen;
	// internet address of the server and the client
	struct sockaddr_in serv_addr, cli_addr;
	// adress of the object used in the thread
	Receiver *threadedServer;
};

#endif
