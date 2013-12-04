#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <iostream>
#include <vector>
#include <Client.h>
#include <BasicServer.h>

using namespace std;

class Connector {
public:
	Connector();
	Connector(unsigned int nbMaxClients, string listIpAdress[], unsigned int clientPortNo[], unsigned int portNo);
	~Connector();

	void send(string ip, string msg);
	void broadcast(string msg);
	
	void addConnection(string ip);



private:
	vector<string> pairs;
	vector<Client> clients;
	BasicServer server;
};

#endif
