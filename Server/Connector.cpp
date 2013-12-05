#include <iostream>
#include <vector>

#include "Connector.h"

Connector::Connector() {

}

Connector::Connector(unsigned int nbMaxClients, string listIpAdress[], unsigned int clientPortNo[], unsigned int portNo) {
	server.serverMain(portNo);		
	printf("Serveur launched\n");

	for (unsigned int i = 0; i < nbMaxClients-1; i++) {
		clients.push_back(Client());
	}	

	bool allConnected = false;
	while (! allConnected) {
		if (! allConnected) {
			printf("Connexion in progress...\n");
		}
		allConnected = true;
		for (unsigned int i = 0; i < nbMaxClients-1; i++) {
			if (! clients[i].isConnected()) {
				clients[i].clientMain(listIpAdress[i], clientPortNo[i]);	
				allConnected = false;
			}
		}
		// attemp to connect every 5 seconds
		sleep(5);
	}
	printf("Client(s) connected\n");
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
