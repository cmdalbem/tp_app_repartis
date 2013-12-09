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
	void initialize(unsigned int nbMaxSenders,vector<string>& listIpAdress, unsigned int senderPortNo[]);
	Connector() {};
	Connector(Server *pserver);
	Connector(int machineId, unsigned int nbMaxSenders, vector<string>& listIpAdress, unsigned int senderPortNo[], unsigned int portNo, Server *pserver, vector<unsigned long long>& othersMachineId);
	~Connector();

	void send(unsigned long long id, string msg);
	void broadcast(string msg);
	
	void addConnection(string ip);
	void subscribe(Sender* s, int nb) { 
		senders.insert(std::pair<int,Sender*>(nb,s));
	}
	void unsubscribe(int id) { senders.erase(id); }
	int getMachineId() {
		return machineId;
	}
	bool isBlacklisted(string ip) {
		return (this->listIp.count(ip));
	}
private:
	set<string> listIp;
	unsigned long long machineId;
	vector<unsigned long long> othersMachineId;
	map<int,Sender *> senders;

	Receiver receiver;
};

#endif
