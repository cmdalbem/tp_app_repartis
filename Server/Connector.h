#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <string>
#include <set>
#include <map>
#include <iostream>
#include <vector>
#include <Sender.h>
#include <Receiver.h>

using namespace std;

#define DEFAULT_TIMEOUT 0.5
using namespace std;
class Server;

class Connector {
public:
	void initialize(unsigned int nbMaxSenders,string listIpAdress[], unsigned int senderPortNo[]);
	Connector() {};
	Connector(Server *pserver);
	Connector(int machineId, unsigned int nbMaxSenders, string listIpAdress[], unsigned int senderPortNo[], unsigned int portNo, Server *pserver);
	~Connector();

	void send(int id, string msg);
	// Blocking receive. Returns "false" if timeout.
	bool receive(string *src, string *msg, float timeout=DEFAULT_TIMEOUT);
	void broadcast(string msg);
	
	void addConnection(string ip);
	void subscribe(Sender* s) { 
		static int nb=0;
		senders.insert(std::pair<int,Sender*>(nb,s));
		 nb++;
	}
	void unsubscribe(int id) { senders.erase(id); }
	int getMachineId() {
		return machineId;
	}
	int isInListIp(string ip) {
		return this->listIp.count(ip);
	}
private:
	set<string> listIp;
	int machineId;
	map<int,Sender *> senders;

	Receiver receiver;
};

#endif
