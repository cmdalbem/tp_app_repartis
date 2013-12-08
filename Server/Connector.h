#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <iostream>
#include <vector>
#include <Sender.h>
#include <Receiver.h>

using namespace std;

#define DEFAULT_TIMEOUT 0.5
class Server;

class Connector {
public:
	void initialize(unsigned int nbMaxSenders,string listIpAdress[], unsigned int senderPortNo[]);
	Connector() {};
	Connector(Server *pserver);
	Connector(int machineId, unsigned int nbMaxSenders, string listIpAdress[], unsigned int senderPortNo[], unsigned int portNo, Server *pserver);
	~Connector();

	void send(int id, string msg);
	void broadcast(string msg);
	
	void addConnection(string ip);
	
	vector<Sender *> senders;
private:
	int machineId;

	Receiver receiver;
};

#endif
