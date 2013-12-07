#ifndef  SERVER_H
#define SERVER_H
#include <iostream>
#include <string>
#include <vector>

#include <jsoncpp/json.h>
#include "File.h"
#include "FilesManager.h"

using namespace std;

class Server {
public:

	Server(); 
	Server(string ip); 
	Server(int machineId, string ip, string listIpAdress[], unsigned int clientPortNo[], unsigned int portNo); 
	~Server();

	// Public getters
	string getIP() { return ip; }

	// Files Management
	//File* newFile(string title, string content);
	void newFile(string title, string content);
	
	void updateFile(int file_id, string title, string content);
	
	void deleteFile(int file_id);
	
	File* readFile(int file_id);

	void listFiles();

	void reestart();

	void handleMessage(char *msg);
	void ping();

	void configure();

private:
	static const string configFile; 
	FilesManager manager;
	Connector connector;

	string ip;

	int lastId; // has to be synchronized by everybody
	// TODO transcript it into parameters
	unsigned int nbMaxErrors; // K
	unsigned int nbMaxClients; // M
};

#endif

