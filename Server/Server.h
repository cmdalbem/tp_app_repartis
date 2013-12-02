#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

#include "File.h"
#include "FilesManager.h"

using namespace std;


class Server {
public:
	static const unsigned int nb_max_errors = 1; //K
	static const unsigned int nb_max_clients = 10; //M
	static const unsigned int nb_servers = 4; //N
	Server(); 
	~Server();

	// Network
	void addServerConnection(string ip);

	// Files
	File newFile(int id, string title, string content);
	
	void updateFile(int id, string title, string content);
	
	void deleteFile(File *f);
	void deleteFile(int file_id);
	
	File readFile(int file_id);

	void listFiles();



private:
	vector<string> otherServers;
	FilesManager manager;

};
