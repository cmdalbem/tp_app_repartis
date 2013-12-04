#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <iostream>
#include <vector>

using namespace std;

class Connector {
public:
	Connector();
	Connector(unsigned int nbMaxClients, string listIpAdress[], unsigned int portNo);
	~Connector();

	void send(string ip, string msg);
	void broadcast(string msg);
	
	void addConnection(string ip);



private:
	vector<string> pairs;

};

#endif
