#ifndef  SERVER_H
#define SERVER_H
#include <iostream>
#include <string>
#include <vector>
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

// Structure of a message stored in the Buffer. These are messages who doesn't have a specific
//   action required by who receives it, but might have differente meanings in each algorithm
//   implementation. So, these messages when received are stored in a buffer, which is consulted
//   by the algorithms that look for specific types.
struct MessageStoreNode {
	int src;
	string type;
	string content;
};

class Server {
public:

	Server(); 
	Server(int machineId, string ip, vector<string> listIpAdress, unsigned int clientPortNo[], unsigned int portNo); 
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

	// Protocol message writing functions
	string msg_file_req(int file_id);
	string msg_who_has(int file_id);
	string msg_i_has(int file_id);
	string msg_restart();
	string msg_del(int file_id);
	string msg_aliveQ(); //alive?
	string msg_aliveA(); //alive!
	string msg_new_file(string title, string content);
	string msg_update_file(string title, string content);
	string msg_delete_file(int file_id);
	string msg_read_file(int file_id);
	string msg_file_transfer(string fileJson);
	string msg_file_transfer(File *f);
	string msg_add(string fileJson);
	string msg_add(File *f);
	
	// Message Buffer managing
	void pushMessage(int src, string msg);
	void pushMessage(int src, string msg, string content);
	bool retrieveMessage(MessageStoreNode *ret, string type);
	bool retrieveMessage(MessageStoreNode *ret, string type, string content);
	bool retrieveMessage(MessageStoreNode *ret, int src, string type);

	// Public getters
	string getIP() { return ip; }
	int getId() { return machineId; }

	void handleMessage(char *msg);
	void ping();

	void configure();

private:
	static const string configFile; 

	vector<MessageStoreNode> msgsBuffer;
	pthread_mutex_t msgsBufferMutex;
	pthread_cond_t msgsBufferCond;

	// id of the machine using this server (often = ip)
	int machineId;

	// starts with 0, increments at each new file.
	// the real file ID will be formed by machineID + lastFileID
	int lastFileId;
	
	string ip;
};

#endif

