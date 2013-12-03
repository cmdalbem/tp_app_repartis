#include <iostream>
#include <string>
#include <vector>

#include "File.h"
#include "FilesManager.h"
#include "Connector.h"

using namespace std;


class Server {
public:
	static const unsigned int nb_max_errors = 1; //K
	static const unsigned int nb_max_clients = 10; //M
	static const unsigned int nb_servers = 4; //N
	Server(); 
	Server(string ip); 
	~Server();

	// Public getters
	getIP() { return ip; }

	// Files Management
	File* newFile(string title, string content);
	
	void updateFile(int id, string title, string content);
	
	void deleteFile(File *f);
	void deleteFile(int file_id);
	
	File* readFile(int file_id);

	void listFiles();



private:
	FilesManager manager;
	Connector connector;

	string ip;

	int lastId; // has to be synchronized by everybody

};
