#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

void error(const char *msg)
{
	perror(msg);
	exit(0);
}

class Client {
	// defines the file descriptor of the socket
	int sockfd;
	// defines the port number 
	int portNo;
	// internet address of the server
	struct sockaddr_in serv_addr;
	// pointer to the computer hosting the server
	struct hostent *server;

	public:
	void setPortNo(int portNo) {
		this->portNo = portNo;
	}

	void createSocket(char * hostName) {
		// create a socket 	AF_INET for Internet Domain
		// 					SOCK_STREAM for a stream
		//					0 is a magic number
		this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
		// test the opening
		if (this->sockfd == -1) 
			error("ERROR opening socket");
		// get the host. Once again, be confident in the input
		this->server = gethostbyname(hostName);
		// test the initialisation
		if (this->server == NULL) {
			fprintf(stderr,"ERROR, no such host\n");
			exit(0);
		}
		// initialize serv_addr
		bzero((char *) &this->serv_addr, sizeof(this->serv_addr));
		// address family -> internet
		this->serv_addr.sin_family = AF_INET;
		// copy the host address
		bcopy((char *)this->server->h_addr, 
				(char *)&this->serv_addr.sin_addr.s_addr,
				this->server->h_length);
		// translate the port into web byte order
		this->serv_addr.sin_port = htons(this->portNo);
	}	

	void connectSocket() {
		if (connect(this->sockfd,(struct sockaddr *) &this->serv_addr,sizeof(this->serv_addr)) < 0) 
			error("ERROR connecting");
	}

	void closeClient() {
		close(this->sockfd);
	}

	void writeMessage(char *buffer) {
		int tmp;
		tmp = write(this->sockfd, buffer, strlen(buffer));
		if (tmp < 0) 
			error("ERROR writing to socket");
	}

	void readMessage(char* buffer) {
		int tmp;
		bzero(buffer,256);
		tmp = read(this->sockfd, buffer, 255);
		if (tmp < 0) 
			error("ERROR reading from socket");
		printf("%s\n", buffer);		
	}

	int clientMain(int argc, char *argv[]) {
		// Captain obvious likes to describe the declaration of the client
		// buffer used for the communication
		char buffer[256];
		// shout at the bad user

		// be confident that your user is not a troll
		this->setPortNo(atoi(argv[2]));
		this->createSocket(argv[1]);

		// it's gonna be a long talk
		this->connectSocket();		

		while(1) {
			printf("Please enter the message: ");
			// initialize & read into the buffer
			bzero(buffer,256);
			fgets(buffer,255,stdin);
			// write to the other computer
			this->writeMessage(buffer);
			// wait for the answer
			this->readMessage(buffer);
		}
		// close the door
		this->closeClient();
	}
};

int main(int argc, char *argv[])
{
	Client client;
	client.clientMain(argc, argv);

	return 0;
}



