#include <iostream>
#include <string>

#include "Server.h"

Server::Server() {
	// do the complex server/client initializations here 
}

Server::~Server() {	

}

////////////////////////
// Network Management //
////////////////////////

void Server::addServerConnection(string ip) {
	// do the complex server/client initializations here
	// ...
}


//////////////////////
// Files Management //
//////////////////////

File Server::newFile(int id, string title, string content) {
	// Add file locally
	manager.add(new File(id, title, content));

	// Replicate it in the network
	// ...

}

void Server::updateFile(int id, string title, string content) {
	// ...

}

void Server::deleteFile(File *f) {
	// ...

}

void Server::deleteFile(int file_id) {
	// ...

}
	
void deleteFile(int file_id) {
	// ...

}
	
File Server::readFile(int file_id) {
	// ...

}

void Server::listFiles() {
	// ...

}
