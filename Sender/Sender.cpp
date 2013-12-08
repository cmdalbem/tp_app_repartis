#include "Sender.h"
#include <sstream>
using namespace std;

// simple constructor with default value
Sender::Sender(int id, string ip) : threadedSender(NULL), connected(false), machineId(id){ 
	
	this->ip = ip;
	pthread_mutex_init(&m, NULL);
	pthread_cond_init(&c, NULL);
};

// simple destructor
Sender::~Sender() {
	closeSender();
}

// set the port number
void Sender::setPortNo(int portNo) {
	this->portNo = portNo;
}

// create the socket and initialize it
void Sender::createSocket(string hostName) {
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
void Sender::connectSocket() {
	if (connect(this->sockfd,(struct sockaddr *) &this->servAddr,sizeof(this->servAddr)) < 0) { 
		connected = false;
		error("ERROR connecting");
	} else {
		connected = true;
	}
}

// has to be called before the destruction
void Sender::closeSender() {
	close(this->sockfd);
}

// write a message to the server
void Sender::writeMessage(char* buffer) {
	int tmp;
	tmp = write(this->sockfd, buffer, strlen(buffer)); 
	if (tmp < 0) { 
		// Do not exit
		perror("ERROR writing to socket");
		//error("ERROR writing to socket");
	}
}

// read a message from the server
void Sender::readMessage(char *buffer) {
	int tmp;
	bzero(buffer,256);
	tmp = read(this->sockfd, buffer, 255);
	if (tmp < 0) 
		error("ERROR reading from socket");
	printf("Sender: %s\n", buffer);		
}

// return true if the Sender is connected, false otherwise
bool Sender::isConnected() {
	if (threadedSender != NULL) {
		return threadedSender->connected; 
	} else {
		return false;
	}
}

void Sender::treatEvent(Event& event)
{
	char buffer[256];
	if (event.getType() == Ping) {
		bzero(buffer,256);
		sprintf(buffer,event.getMessage().c_str());
		this->writeMessage(buffer);
	}

}



// everything needed to run a server
void *Sender::run() {
	this->connectSocket();		
	// buffer used for the communication

	// it's gonna be a long talk
	Event event(Ping,"PING");
	while(1) {
		// initialize & read into the buffer
		this->getEvent(event);
		this->treatEvent(event);
		sleep(5);
	}

	closeSender();

	return 0;
}

// wrapper to launch a thread
void *Sender::runWrapper(void *context) {
	return ((Sender *) context)->run();
}

// launch a new Sender on a thread
int Sender::SenderMain(string hostName,int portNo) {
	// be confident that your user is not a troll
	this->setPortNo(portNo);
	this->createSocket(hostName);

	// copy the object for the thread
	this->threadedSender = new Sender(*this);
	pthread_t t;
	pthread_create(&t, NULL, &Sender::runWrapper, this->threadedSender);
	// close the door
	//this->closeSender();
	return 0;
}

void Sender::update(Event& what) {
	pthread_mutex_lock(&m);
	bool emptyQueue = eventQueue.empty();
	this->eventQueue.push_back(what);
	if (emptyQueue) {
		pthread_cond_signal(&c);
	}
	pthread_mutex_unlock(&m);
}

void Sender::getEvent(Event& event) {
	/* if the Sender is getting an event, the Server must wait before he add an event */
	pthread_mutex_lock(&m);
	if ( eventQueue.empty() ) {
		/* release the mutex m if the queue is empty*/
		pthread_cond_wait(&c,&m);
	}
	event = eventQueue.front(); 

	stringstream ss;
	ss << "from 502"<<this->machineId << " to "<< this->portNo;
	string id = ss.str();
	event.setMessage("Message " + id + " " + event.getMessage());
	cout << "Sending Message " << id << " " << event.getMessage() << endl; 
	eventQueue.pop_front();
	pthread_mutex_unlock(&m);
}


void Sender::error(const char * msg) {
	connected = false;
	perror(msg);
	pthread_exit(NULL);
}
