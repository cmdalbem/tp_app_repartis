#include <iostream>
#include <vector>

#include "Connector.h"
#include "Server.h"
#include "Event.h"

using namespace std;

Connector::Connector(Server *pserver) {
	 receiver = Receiver(pserver); 
}

void Connector::initialize(unsigned int nbMaxSenders,vector<string> listIpAdress, unsigned int senderPortNo[]) {
	bool allConnected;
	bool change;
	for (unsigned int i=0; i<nbMaxSenders-1;i++) {
		this->listIp.insert(listIpAdress[i]);
	}
	while(1) {
		allConnected = false;
		change = false;
		while (! allConnected) {
			allConnected = true;
			for (unsigned int i = 0; i < nbMaxSenders-1; i++) {
				if (! senders[i]->isConnected()) {
					cout << "connecting a sender" << endl;
					senders[i]->SenderMain(listIpAdress[i], senderPortNo[i]);	
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

Connector::Connector(int machineId, unsigned int nbMaxSenders, vector<string> listIpAdress, unsigned int senderPortNo[], unsigned int portNo, Server *pserver) {
	this->machineId = machineId;
	this->firstConnectionComplete=false;

	receiver=Receiver(pserver);
	receiver.serverMain(machineId, portNo);		
	printf("Server launched\n");

	for (unsigned int i = 0; i < nbMaxSenders-1; ++i) {
		cout << i+1 << " senders registered of " << nbMaxSenders -1 << endl;
		this->subscribe(new Sender(machineId,listIpAdress[machineId]));
	}	
	cout << "the " << nbMaxSenders << " have been registered" << endl;
}

Connector::~Connector() {

}

void Connector::send(int destId, string msg) {
	int offset;
	offset = (machineId < destId) ? -1 : 0;
	if (destId < senders.size()) {
		Event e(BetweenServer, msg);
		senders[destId + offset]->threadedSender->update(e);
	} else {
		fprintf(stderr, "Envoie à ID inexistant : %i\n", destId);
		exit(0);
	}
}

void Connector::broadcast(string msg) {
	//...
	msg = msg;
}

void Connector::addConnection(string ip) {
	//...
	ip = ip;	
}
