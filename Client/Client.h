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
	Client() : connected(false), threadedClient(NULL) {};
	Client(int id) : connected(false), machineId(id), threadedClient(NULL) 
	{
		pthread_mutex_init(&m, NULL);
		pthread_cond_init(&c, NULL);
	};

	void setPortNo(int portNo);

	void createSocket(std::string hostName);

	void connectSocket(); 

	void closeClient();

	void writeMessage(char* buffer);

	void readMessage(char* buffer);

	void *run(); 

	bool isConnected();

	static void *runWrapper(void *context);

	int clientMain(std::string hostName, int portNo);
	bool connected;
	void update(Publisher* who, Event& what); 
	void getEvent(Event& event); 

private:
	list<Event> eventQueue;
	pthread_mutex_t m;
	pthread_cond_t c;

	int machineId;
	// defines the file descriptor of the socket
	int sockfd;
	// defines the port number 
	int portNo;
	// internet address of the server
	struct sockaddr_in serv_addr;
	// pointer to the computer hosting the server
	struct hostent *server;
	Client *threadedClient;
	// 

	void error(const char * msg) {
		connected = false;
		perror(msg);
		pthread_exit(NULL);
	}
};

#endif
