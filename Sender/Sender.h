#ifndef Sender_H
#define Sender_H

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


class Sender : public Subscriber {
public:
	// simple constructor with default value
	Sender(unsigned long long id);
	// simple destructor
	~Sender(); 
	// return true if the Sender is connected, false otherwise
	bool isConnected();
	// launch a new Sender on a thread
	int SenderMain(std::string hostName, int portNo);
	// everything needed to run a server
	void *run(); 

	void update(Event& what); 
	void getEvent(Event& event); 
	void treatEvent(Event& event);

	// object used in the thread
	Sender *threadedSender;
private:
	// set the port number
	void setPortNo(int portNo);
	// create the socket and initialize it
	void createSocket(std::string hostName);
	// connect the socket to the server
	void connectSocket(); 
	// has to be called before the destruction
	void closeSender();
	// write a message to the server
	void writeMessage(char* buffer);
	// read a message from the server
	void readMessage(char* buffer);
	// wrapper to launch a thread
	static void *runWrapper(void *context);
	// used to signal an error & quit the thread
	void error(const char * msg); 

	// indicates wheter the Sender is connected or not
	bool connected;
	// id of the machine using the Sender
	unsigned long long  machineId;
	// defines the file descriptor of the socket
	int sockfd;
	// defines the port number 
	int portNo;
	// internet address of the server
	struct sockaddr_in servAddr;
	// pointer to the computer hosting the server
	struct hostent *server;

	list<Event> eventQueue;
	pthread_mutex_t m;
	pthread_cond_t c
	;
};

#endif
