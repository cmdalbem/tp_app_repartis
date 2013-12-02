#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

#include "File.h"

using namespace std;


class Server {

public:
	Server(); 
	~Server();

	void addServerConnection(string ip);

	File newFile();
	void updateFile(File f);
	void deleteFile(File f);


private:
	vector<string> otherServers;
	vector<File> files;

};