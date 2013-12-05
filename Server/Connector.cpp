#include <iostream>
#include <vector>

#include "Connector.h"

Connector::Connector() {

}

Connector::Connector(int machineId, unsigned int nbMaxClients, string listIpAdress[], unsigned int clientPortNo[], unsigned int portNo) {
	server.serverMain(machineId, portNo);		
	printf("Serveur launched\n");

	for (unsigned int i = 0; i < nbMaxClients-1; i++) {
		clients.push_back(Client(machineId));
	}	
 
	bool allConnected;
	bool change;
	while(1) {
	allConnected = false;
	change = false;
	while (! allConnected) {
		allConnected = true;
		for (unsigned int i = 0; i < nbMaxClients-1; i++) {
			if (! clients[i].isConnected()) {
				clients[i].clientMain(listIpAdress[i], clientPortNo[i]);	
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

string Connector::receive() {
	//...
	
}

void Connector::broadcast(string msg) {
	//...
	msg = msg;
}
	
void Connector::addConnection(string ip) {
	//...
	ip = ip;	
}
