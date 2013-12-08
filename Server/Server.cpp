#include <iostream>
#include <string>
#include <Event.h>
#include "Connector.h"
#include "Server.h"
#include <cmath>
#include <fstream>

using namespace std;

const string Server::configFile = "system.cfg";

void Server::configure() {
	lastFileId = 0;

	ifstream file;
	file.open(this->configFile.c_str());
	if (! file.is_open()) {
		file.open(("../" +this->configFile).c_str());
	}
	if (! file.is_open()) {
		printf("Error on configuration file %s opening\n", configFile.c_str());
		exit(0);
	}

	string tmp;
	char buffer[200];
	file.getline(buffer, 200);
	file >> nbMaxErrors;  
	file.getline(buffer, 200);
	file.getline(buffer, 200);
	file >> nbMaxClients;
} 

Server::Server(string ip) {
	configure();
	this->connector = Connector(this); 
	this->ip = ip;
	lastFileId = 0;
}	


Server::Server(int machineId, string ip, string listIpAdress[], unsigned int clientPortNo[], unsigned int portNo) {
	cout << "entering Server" << endl;
	configure();
	this->ip = ip;
	this->machineId = machineId;
	// do the complex server/client initializations here 
	Connector(machineId, nbMaxClients, listIpAdress, clientPortNo, portNo, this);
	cout << "quitting Server" << endl;
} 

Server::~Server() {	

}

void Server::ping() {
	cout << "Sending a ping" << endl;
	Event event = Event(Ping,"Ping");
	this->connector.senders[0]->update(event);
}

//////////////////////
// Files Management //
//////////////////////

// returns the integer concatenation of the machineId with the lastFileId
int Server::getNewFileId()
{
	#define COUNT_DIGITS(x) ((int) log10(x)+1)

	// If machineId is '127001' and lastFileId is '666', then newFileId => 127001666
	int newFileId = machineId * pow(10,COUNT_DIGITS(lastFileId)) + lastFileId;
	return newFileId;
}

//File* Server::newFile(string title, string content) {
void Server::newFile(string title, string content) {
	string msg;

	// Create new file
	File* f = new File(getNewFileId(), title, content);
	f->to_JSON(msg);
	msg = "f_" + msg;

	// Finds out who is alive
	msg = "alive?";
	connector.broadcast(msg);

	// Replicate it in the network
	string src;
	for (unsigned int i = 0; i < nbMaxErrors+1; ++i) {
		// Send the file for the first K+1 guys who answers
		connector.receive(&src,&msg);
		connector.send(src,msg);
	}

	// Add file locally
	manager.add(f);

	lastFileId++;
}

void Server::updateFile(int file_id, string title, string content) {
	string msg;

	// Asks who has the file
	msg = "who_has " + file_id;
	connector.broadcast(msg);

	// Waits until someone answers
	string src;
	File* f = new File(file_id, title, content);
	f->to_JSON(msg);
	msg = "f_" + msg;

	while (connector.receive(&src,&msg))
		connector.send(src,msg);

	// Check if the file is available locally
	File* file = manager.read(file_id);
	if(file)
		manager.modify(file_id, title, content);

}

void Server::deleteFile(int file_id) {
	string msg;
		
	// Tells everyone that this file has to be deleted
	msg = "del " + file_id;
	connector.broadcast(msg);

	// Check if the file is available locally
	File* file = manager.read(file_id);
	if(file)
		manager.erase(file_id);

}
	
File* Server::readFile(int file_id) {
	File* file = NULL;

	// Check if the file is available locally
	file = manager.read(file_id);
	if(file)
		return file;
	else {	
		string msg;
		
		// Asks who has the file
		msg = "who_has " + file_id;
		connector.broadcast(msg);

		// Waits until someone answers
		string src;
		connector.receive(&src,&msg);

		// Asks the file for the first who answered
		msg = "file_req " + file_id;
		connector.send(src,msg);

		// Receive the file
		connector.receive(&src,&msg);

		file = new File();
		file->parse_JSON(msg);

		return file;
	}

}

void Server::listFiles() {
	// TO DO: how to retrieve a full list of files through servers?
	
	// temporary: just list local files for now
	manager.showAll();
}

void Server::reestart() {
	// TODO: make it select only one machine to receive a given file from.
	string msg = "reestart";
	connector.broadcast(msg);

	string src;
	while (connector.receive(&src,&msg)) {
		File* f = new File();
		f->parse_JSON(msg);
		manager.add(f);
	}

}

void Server::handleMessage(char *msg) {
	cout<<"handleMessage"<<endl;
	ping();
}
