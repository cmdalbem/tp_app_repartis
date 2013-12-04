/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#ifndef BASICSERVER_H
#define BASICSERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <pubsub.h>
#include <pthread.h>


class BasicServer : public Publisher {
public:

	void initialize(int portNo);
	void connectServer(); 
	void writeMessage(char * msg);
	void readMessage();
	void process();
	void serverMain(int portNo);
	void *run(); 
	static void *runWrapper(void *context);

private:
	// defines two file descriptor and the port number
	int sockfd, newsockfd, portno;
	// size of the client
	socklen_t clilen;
	// internet address of the server and the client
	struct sockaddr_in serv_addr, cli_addr;

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
};



/*
   int main(int argc, char *argv[])
   {
   Server server;
   server.serverMain(argc, argv);

   return 0; 
   }
 */

#endif

