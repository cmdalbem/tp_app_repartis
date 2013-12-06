#include "Client.h"

// simple constructor with default value
Client(int id) : connected(false), machineId(id), threadedClient(NULL) 	{
	pthread_mutex_init(&m, NULL);
	pthread_cond_init(&c, NULL);
};

// simple destructor
Client::~Client() {
	closeClient();
}

// set the port number
void Client::setPortNo(int portNo) {
	this->portNo = portNo;
}

// create the socket and initialize it
void Client::createSocket(string hostName) {
	// create a socket 	AF_INET for Internet Domain
	//	 		SOCK_STREAM for a stream
	//			0 is a magic number
	this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
	// test the opening
	if (this->sockfd == -1) 
		error("ERROR opening socket");
	// get the host
	this->server = gethostbyname(hostName.c_str());
	// test the initialisation
	if (this->server == NULL) {
		fprintf(stderr,"ERROR, no such host\n");
		exit(0);
	}
	// initialize servAddr
	bzero((char *) &this->servAddr, sizeof(this->servAddr));
	// address family -> internet
	this->servAddr.sin_family = AF_INET;
	// copy the host address
	bcopy((char *)this->server->h_addr, 
			(char *)&this->servAddr.sin_addr.s_addr,
			this->server->h_length);
	// translate the port into web byte order
	this->servAddr.sin_port = htons(this->portNo);
}	

// connect the socket to the server
void Client::connectSocket() {
	if (connect(this->sockfd,(struct sockaddr *) &this->servAddr,sizeof(this->servAddr)) < 0) { 
		connected = false;
		error("ERROR connecting");
	} else {
		connected = true;
	}
}

// has to be called before the destruction
void Client::closeClient() {
	close(this->sockfd);
}

// write a message to the server
void Client::writeMessage(char* buffer) {
	int tmp;
	tmp = write(this->sockfd, buffer, strlen(buffer)); 
	if (tmp < 0) { 
		// Do not exit
		perror("ERROR writing to socket");
		//error("ERROR writing to socket");
	}
}

// read a message from the server
void Client::readMessage(char *buffer) {
	int tmp;
	bzero(buffer,256);
	tmp = read(this->sockfd, buffer, 255);
	if (tmp < 0) 
		error("ERROR reading from socket");
	printf("Client: %s\n", buffer);		
}

// return true if the client is connected, false otherwise
bool Client::isConnected() {
	if (threadedClient != NULL) {
		return threadedClient->connected; 
	} else {
		return false;
	}
}

// everything needed to run a server
void *Client::run() {
	this->connectSocket();		
	// buffer used for the communication
	char buffer[256];

	// it's gonna be a long talk
	while(1) {
		// initialize & read into the buffer
		bzero(buffer,256);
		for(int i = 0; i < 255; i++) {
			buffer[i] = "X -> Y: Hello world!"[i];
		}
		buffer[0] = machineId + '0';
		buffer[5] = portNo - 5020 + '0';
//		fgets(buffer,255,stdin);
		// write to the other computer
		this->writeMessage(buffer);
		// wait for the answer
		this->readMessage(buffer);
		sleep(10);
	}

	closeClient();

	return 0;
}

// wrapper to launch a thread
void *Client::runWrapper(void *context) {
	return ((Client *) context)->run();
}

// launch a new client on a thread
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

void Client::update(Publisher* who, Event& what) {
	pthread_mutex_lock(&m);
	bool emptyQueue = eventQueue.empty();
	this->eventQueue.push_back(what);
	if (emptyQueue) {
		pthread_cond_signal(&c);
	}
	pthread_mutex_unlock(&m);
}

void Client::getEvent(Event& event) {
	/* if the Client is getting an event, the Server must wait before he add an event */
	pthread_mutex_lock(&m);
	if ( eventQueue.empty() ) {
		/* release the mutex m if the queue is empty*/
		pthread_cond_wait(&c,&m);
	}
	event = eventQueue.front(); 
	eventQueue.pop_front();
	pthread_mutex_unlock(&m);
}

void Client::error(const char * msg) {
	connected = false;
	perror(msg);
	pthread_exit(NULL);
}
