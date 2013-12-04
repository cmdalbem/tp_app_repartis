#include <iostream>
#include <vector>

#include "Connector.h"

Connector::Connector() {

}

Connector::Connector(unsigned int nbMaxClients, string listIpAdress[], unsigned int clientPortNo[], unsigned int portNo) {
	server.serverMain(portNo);		

	for (unsigned int i = 0; i < nbMaxClients; i++) {
		clients.push_back(Client());
	}	
	bool allConnected = false;
	while (! allConnected) {
		allConnected = true;
		for (unsigned int i = 0; i < nbMaxClients; i++) {
			if (! clients[i].isConnected()) {
				clients[i].clientMain(listIpAdress[i], clientPortNo[i]);	
				allConnected = false;
			}
		}
	}
}

Connector::~Connector() {

}

void Connector::send(string ip, string msg) {
	//...

}

void Connector::broadcast(string msg) {
	//...

}
	
void Connector::addConnection(string ip) {
	//...
	
}
