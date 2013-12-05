#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <iostream>
#include <vector>
#include <Client.h>
#include <BasicServer.h>

using namespace std;

#define DEFAULT_TIMEOUT 0.5

class Connector {
public:
	Connector();
	Connector(int machineId, unsigned int nbMaxClients, string listIpAdress[], unsigned int clientPortNo[], unsigned int portNo);
	~Connector();

	void send(string ip, string msg);
	// Blocking receive. Returns "false" if timeout.
	bool receive(string *src, string *msg, float timeout=DEFAULT_TIMEOUT);
	void broadcast(string msg);
	
	void addConnection(string ip);



private:
	vector<string> pairs;
	vector<Client> clients;
	BasicServer server;
};

#endif
