#include <iostream>
#include <vector>

#include "Connector.h"

Connector::Connector() {

}

Connector::Connector(unsigned int nbMaxClients, string listIpAdress[], unsigned int portNo) {

	for (unsigned int i = 0; i < nbMaxClients; i++) {
		clients.push_back(Client());
		clients[i].clientMain(listIpAdress[i], portNo);	
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
