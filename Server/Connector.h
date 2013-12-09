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
/* The Connector manages all the Sender and the Receivers of the Server,
   he is used to transfer the message from the receiver to the right Sender */

class Connector {
public:
	/* Loop in order to connect all the senders */
	void initialize(unsigned int nbMaxSenders,vector<string>& listIpAdress, unsigned int senderPortNo[], Server *pserver);
	/* Constructors */
	Connector() {};
	Connector(Server *pserver);
	Connector(int machineId, unsigned int nbMaxSenders, vector<string>& listIpAdress, unsigned int senderPortNo[], unsigned int portNo, Server *pserver, vector<unsigned long long>& othersMachineId);
	~Connector();
	/* sending to a precise machine a message */
	void send(unsigned long long id, string msg);
	/* sending a message to all the other servers */
	void broadcast(string msg);
	/* not implemented */
	void addConnection(string ip);
	/* subscribe the Sender to a publisher interface */
	void subscribe(Sender* s, int nb) { 
		senders.insert(std::pair<int,Sender*>(nb,s));
	}
	/* unsubscribe them from this interface (if there was a timeout*/
	void unsubscribe(int id) { senders.erase(id); }
	/* getter of the unique id of the machine */
	int getMachineId() {
		return machineId;
	}
	/* to handle the connexion of others sender than the servers one */
	bool isBlacklisted(string ip) {
		return (this->listIp.count(ip));
	}
private:
	/* set of all the ip used in the distributed system */
	set<string> listIp;
	/* id of the machine */
	unsigned long long machineId;
	/* vector of all the other machine id */
	vector<unsigned long long> othersMachineId;
	/* all the senders that this Server possess */
	map<int,Sender *> senders;
	/* the Receiver waiting for connexion and threading to access new ones */
	Receiver receiver;
};

#endif
