#include <iostream>
#include <string>
#include <vector>

#include <jsoncpp/json.h>
#include "File.h"
#include "FilesManager.h"
#include "Connector.h"

using namespace std;


class Server {
public:
	// TODO transcript it into parameters
	static const unsigned int nb_max_errors = 1; //K
	static const unsigned int nb_max_clients = 2; //M
	static const unsigned int nb_servers = 2; //N

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



private:
	FilesManager manager;
	Connector connector;

	string ip;

	int lastId; // has to be synchronized by everybody

};
