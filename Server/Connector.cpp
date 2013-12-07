#include <iostream>
#include <vector>

#include "Connector.h"
#include "Server.h"
#include "Event.h"

Connector::Connector(Server *pserver) {
	 receiver = Receiver(pserver); 
}

Connector::Connector(int machineId, unsigned int nbMaxSenders, string listIpAdress[], unsigned int senderPortNo[], unsigned int portNo, Server *pserver) {
	receiver=Receiver(pserver);
	receiver.serverMain(machineId, portNo);		
	printf("Serveur launched\n");

	for (unsigned int i = 0; i < nbMaxSenders-1; i++) {
		senders.push_back(Sender(machineId));
	}	
 
	bool allConnected;
	bool change;
	while(1) {
		allConnected = false;
		change = false;
		while (! allConnected) {
			allConnected = true;
			for (unsigned int i = 0; i < nbMaxSenders-1; i++) {
				if (! senders[i].isConnected()) {
					senders[i].SenderMain(listIpAdress[i], senderPortNo[i]);	
					allConnected = false;
				}
			}
			if (! allConnected) {
				change = true;
				printf("Connexion in progress...\n");
			}
			// attemp to connect every x seconds
			sleep(5);
		}
		if (change) {
			printf("Client(s) connected\n");
			Event event = Event(Ping,"ping");
			senders[0].update(event);
		}
	}
}

Connector::~Connector() {

}

void Connector::send(string ip, string msg) {
	//...
	ip = ip;
	msg = msg;
}

bool Connector::receive(string *src, string *msg, float timeout) {
	//...

	// returns "false" if timeout
	return false;

	return true;
}

void Connector::broadcast(string msg) {
	//...
	msg = msg;
}
	
void Connector::addConnection(string ip) {
	//...
	ip = ip;	
}
