#include <iostream>
#include <string>

#include "Server.h"
#include "Connector.h"

Server::Server(string ip) {
	this->ip = ip;
}	


Server::Server(int machineId, string ip, string listIpAdress[], unsigned int clientPortNo[], unsigned int portNo) {
	this->ip = ip;
	// do the complex server/client initializations here 
	Connector(machineId, nb_max_clients, listIpAdress, clientPortNo, portNo);
} 

Server::~Server() {	

}

//////////////////////
// Files Management //
//////////////////////

//File* Server::newFile(string title, string content) {
void Server::newFile(string title, string content) {
	string msg;

	// Create new file
	File* f = new File(lastId, title, content);
	f->to_JSON(msg);

	// Finds out who is alive
	msg = "alive?";
	connector.broadcast(msg);

	// Replicate it in the network
	string src;
	for (int i = 0; i < nb_max_errors+1; ++i) {
		// Send the file for the first K+1 guys who answers
		connector.receive(&src,&msg);
		connector.send(src,msg);
	}

	// Add file locally
	manager.add(f);

	// TEMPORARY
	lastId++;

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
	
	// just list local files for now
	manager.showAll();
}
