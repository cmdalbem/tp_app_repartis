#include "Client.h"

void Client::setPortNo(int portNo) {
	this->portNo = portNo;
}

void Client::createSocket(string hostName) {
	// create a socket 	AF_INET for Internet Domain
	// 					SOCK_STREAM for a stream
	//					0 is a magic number
	this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
	// test the opening
	if (this->sockfd == -1) 
		error("ERROR opening socket");
	// get the host. Once again, be confident in the input
	this->server = gethostbyname(hostName.c_str());
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

void Client::connectSocket() {
	if (connect(this->sockfd,(struct sockaddr *) &this->serv_addr,sizeof(this->serv_addr)) < 0) { 
		connected = false;
		error("ERROR connecting");
	} else {
		connected = true;
	}
}

void Client::closeClient() {
	close(this->sockfd);
}

void Client::writeMessage(char* buffer) {
	int tmp;
	tmp = write(this->sockfd, buffer, strlen(buffer)); 
	if (tmp < 0) { 
		// Do not exit
		perror("ERROR writing to socket");
		//error("ERROR writing to socket");
	}
}

void Client::readMessage(char *buffer) {
	int tmp;
	bzero(buffer,256);
	tmp = read(this->sockfd, buffer, 255);
	if (tmp < 0) 
		error("ERROR reading from socket");
	printf("Client: %s\n", buffer);		
}

bool Client::isConnected() {
	if (threadedClient != NULL) {
//		printf("%b\n", threadedClient->connected);
		return threadedClient->connected; 
	} else {
		return false;
	}
}

void *Client::run() {
	this->connectSocket();		
	// buffer used for the communication
	char buffer[256];

	// it's gonna be a long talk
	while(1) {
		// initialize & read into the buffer
		bzero(buffer,256);
		for(int i = 0; i < 255; i++) {
			buffer[i] = "Hello world!"[i];
		}
		buffer[255] = '\n';
//		fgets(buffer,255,stdin);
		// write to the other computer
		this->writeMessage(buffer);
		// wait for the answer
		this->readMessage(buffer);
		sleep(5);
	}
}

static void *Client::runWrapper(void *context) {
	return ((Client *) context)->run();
}

int Client::clientMain(string hostName,int portNo) {
	// be confident that your user is not a troll
	this->setPortNo(portNo);
	this->createSocket(hostName);

	// copy the object for the thread
	this->threadedClient = new Client(*this);
	pthread_t t;
	pthread_create(&t, NULL, &Client::runWrapper, this->threadedClient);
	// close the door
	//this->closeClient();
	return 0;
}

