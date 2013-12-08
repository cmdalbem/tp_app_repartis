#ifndef  SERVER_H
#define SERVER_H
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <pthread.h>

#include <jsoncpp/json.h>
#include "File.h"
#include "FilesManager.h"

using namespace std;

struct newFile_args {
	Server *server;
	string title;
	string content;
	vector<int> owners;
};

struct updateFile_args {
	Server *server;
	int file_id;
	string title;
	string content;
	vector<int> owners;
};

struct file_args {
	Server *server;
	int file_id;
};

void 	*newFile(void* args); // args = newFile_args
void 	*updateFile(void* args); // args = updateFile_args
void 	*deleteFile(void* args); // args = file_args
void 	*readFile(void* args); // args = file_args
void 	*listFiles(void* args); // args = Server*
void 	*restart(void* args); // args = Server*


class Server {
public:

	Server(); 
	Server(int machineId, string ip, string listIpAdress[], unsigned int clientPortNo[], unsigned int portNo); 
	~Server();

	// Lots of these functions must be public because they are accessed by the
	//   algorithms threads, which are not methods of the class.

	// Public attributes
	FilesManager manager;
	Connector connector;
	unsigned int nbMaxErrors; // K
	unsigned int nbMaxClients; // M

	// generates an ID for a new file
	int getNewFileId();

	void incrementLastFileId() { lastFileId++; }

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
	
	// Public getters
	string getIP() { return ip; }
	int getId() { return machineId; }

	void handleMessage(char *msg);
	void ping();

	void configure();

private:
	static const string configFile; 

	queue<string> msgQueue;
	pthread_mutex_t msgQueueMutex;
	pthread_cond_t msgQueueCond;

	// id of the machine using this server (often = ip)
	int machineId;

	// starts with 0, increments at each new file.
	// the real file ID will be formed by machineID + lastFileID
	int lastFileId;
	
	string ip;
};

#endif

