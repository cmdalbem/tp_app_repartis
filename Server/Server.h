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
	int getId() { return machineId; }

	// Files Management
	void 	newFile(string title, string content, vector<int> owners);
	void 	updateFile(int file_id, string title, string content, vector<int> owners);
	void 	deleteFile(int file_id);
	File* 	readFile(int file_id);
	void 	listFiles();

	void restart();

	void handleMessage(char *msg);
	void ping();

	void configure();

private:
	static const string configFile; 
	FilesManager manager;
	Connector connector;

	// generates the ID for a new file
	int getNewFileId();

	// id of the machine using this server (often = ip)
	int machineId;

	// starts with 0, increments at each new file.
	// the real file ID will be formed by machineID + lastFileID
	int lastFileId;
	
	string ip;

	// Message writing functions
	string msg_file_req(int file_id);
	string msg_who_has(int file_id);
	string msg_i_has(int file_id);
	string msg_reestart();
	string msg_del(int file_id);
	string msg_aliveQ(); //alive?
	string msg_aliveA(); //alive!
	string msg_new_file(string title, string content);
	string msg_update_file(string title, string content);
	string msg_delete_file(int file_id);
	string msg_read_file(int file_id);
	string msg_file_transfer(string fileJson);
	string msg_file_transfer(File *f);

	// TODO transcript it into parameters
	unsigned int nbMaxErrors; // K
	unsigned int nbMaxClients; // M
};

#endif

