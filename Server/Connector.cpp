#include <iostream>
#include <vector>
#include <algorithm>

#include "Connector.h"
#include "Server.h"
#include "Event.h"
#include <interface.h>

using namespace std;

Connector::Connector(Server *pserver) {
	 receiver = Receiver(pserver); 
}

void Connector::initialize(unsigned int nbMaxSenders,vector<string>& listIpAdress, unsigned int senderPortNo[], Server *pserver) {
	bool allConnected;
	bool change;
	for (unsigned int i=0; i<nbMaxSenders-1;i++) {
		this->listIp.insert(listIpAdress[i]);
	}
	int first=0;
	while(1) {
		allConnected = false;
		change = false;
		while (! allConnected) {
			allConnected = true;
			for (unsigned int i = 0; i < nbMaxSenders-1; i++) {
				if (! senders[this->othersMachineId[i]]->isConnected()) {
					cout << "connecting a sender" << endl;
					senders[this->othersMachineId[i]]->SenderMain(listIpAdress[i], senderPortNo[i]);	
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
		first++;
		if (first == 1)
			cout << "create a file" << endl;
			interfaceMain((void*) pserver);
	}

}

Connector::Connector(int machineId, unsigned int nbMaxSenders, vector<string>& listIpAdress, unsigned int senderPortNo[], unsigned int portNo, Server *pserver, vector<unsigned long long>& othersMachineId) {
	this->machineId = machineId;
	for (int i=0; i < othersMachineId.size(); ++i) {
		this->othersMachineId.push_back(othersMachineId[i]);
	}
	
	receiver=Receiver(pserver);
	receiver.serverMain(machineId, portNo);		
	printf("Server launched\n");

	for (unsigned int i = 0; i < nbMaxSenders-1; ++i) {
		cout << i+1 << " senders registered of " << nbMaxSenders -1 << endl;
		this->subscribe(new Sender(machineId),othersMachineId[i]);
	}	
	cout << "the " << nbMaxSenders << " have been registered" << endl;
}

Connector::~Connector() {

}

// Sends the message 'msg' to the machine identified by 'destId'.
// In low-level, it registers an event with the msg in the pub/sub system to be
//   catched by a sender thread.
void Connector::send(unsigned long long destId, string msg) {
	std::vector<unsigned long long>::iterator it;
	for ( it = this->othersMachineId.begin(); it != this->othersMachineId.end(); ++it) { 
		if (*it == destId) {
			Event e(BetweenServer, msg);
			senders[destId]->threadedSender->update(e);
			return;
		}
	}
	cout << destId << " is not an available id" << endl;
}

// Sends the message 'msg' to all servers registered.
// In low-level, it registers an event with the msg for each Sender registered.
void Connector::broadcast(string msg) {
	std::vector<unsigned long long>::iterator it;
	for (it = this->othersMachineId.begin(); it != this->othersMachineId.end(); ++it) {
		Event e(BetweenServer, msg);
		senders[*it]->threadedSender->update(e);
	}
}

void Connector::addConnection(string ip) {
	//...
	ip = ip;
}
