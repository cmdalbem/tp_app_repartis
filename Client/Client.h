#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <pubsub.h>
#include <Event.h>
#include <pthread.h>


class Client : public Subscriber {
public:
	// simple constructor with default value
	Client(int id);
	// simple destructor
	~Client(); 
	// return true if the client is connected, false otherwise
	bool isConnected();
	// launch a new client on a thread
	int clientMain(std::string hostName, int portNo);
	// everything needed to run a server
	void *run(); 

	void update(Publisher* who, Event& what); 
	void getEvent(Event& event); 

private:
	// set the port number
	void setPortNo(int portNo);
	// create the socket and initialize it
	void createSocket(std::string hostName);
	// connect the socket to the server
	void connectSocket(); 
	// has to be called before the destruction
	void closeClient();
	// write a message to the server
	void writeMessage(char* buffer);
	// read a message from the server
	void readMessage(char* buffer);
	// wrapper to launch a thread
	static void *runWrapper(void *context);
	// used to signal an error & quit the thread
	void Client::error(const char * msg) {

	// id of the machine using the client
	int machineId;
	// defines the file descriptor of the socket
	int sockfd;
	// defines the port number 
	int portNo;
	// internet address of the server
	struct sockaddr_in servAddr;
	// pointer to the computer hosting the server
	struct hostent *server;
	// object used in the thread
	Client *threadedClient;
	// indicates wheter the client is connected or not
	bool connected;

	list<Event> eventQueue;
	pthread_mutex_t m;
	pthread_cond_t c;
};

#endif
