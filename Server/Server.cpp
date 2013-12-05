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
	// Replicate it in the network
	// ...

	// Add file locally
	manager.add(new File(lastId, title, content));

	// TEMPORARY
	lastId++;

}

void Server::updateFile(int id, string title, string content) {
	// ...

	manager.modify(id, title, content);

}

void Server::deleteFile(File *f) {
	// ...

	manager.erase(f);

}

void Server::deleteFile(int file_id) {
	// ...

	manager.erase(file_id);

}
	
File* Server::readFile(int file_id) {
	File* file = NULL;

	// Check if the file is avaiable locally
	file = manager.read(file_id);
	if(file)
		return file;
	else {	
		string msg;
		
		// Asks who has the file
		sprintf(msg,"who_has %i",file_id);
		broadcast(msg);

		// Waits until someone answers
		string src;
		receive(&src,&msg);

		// Asks the file for the first who answered
		sprintf(msg,"file_req %i",file_id);
		send(src,msg);

		// Receive the file
		receive(&src,&msg);

		// TODO: Parse the file...

		return file;
	}

}

void Server::listFiles() {
	
	// TO DO: how to retrieve a full list of files through servers?
	
	// just list local files for now
	manager.showAll();
}
