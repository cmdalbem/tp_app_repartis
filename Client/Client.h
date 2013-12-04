#ifndef CLIENT_H
#define CLIENT_H


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <pubsub.h>

void error(const char *msg)
{
	perror(msg);
	exit(0);
}

class Client : public Subscriber {
	// defines the file descriptor of the socket
	int sockfd;
	// defines the port number 
	int portNo;
	// internet address of the server
	struct sockaddr_in serv_addr;
	// pointer to the computer hosting the server
	struct hostent *server;

	public:
	void setPortNo(int portNo);

	void createSocket(char * hostName);

	void connectSocket(); 

	void closeClient();

	void writeMessage(char *buffer);

	void readMessage(char* buffer);

	int clientMain(int argc, char *argv[]);
};
/*
int main(int argc, char *argv[])
{
	Client client;
	client.clientMain(argc, argv);

	return 0;
}
*/


#endif
