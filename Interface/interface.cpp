#define INTERFACE_H
#include <iostream>
#include <string>

#include "interface.h"

using namespace std;


// Globals
Server *server = NULL;


int main(void) {
	int command;
	
	bool exit = false;
	
	while (not exit) {

		// Connects to a server
		while (server==NULL)
			connectToServer();

		// Server is connected, we can start playing with files
		cout << "(1) to create a file\n"
				"(2) to delete a file\n"
				"(3) to read a file\n"
				"(4) to modify a file\n"
				"(5) show all files\n"
				"(6) to exit\n";

		cin >> command;

		switch(command) {
		case 1:
			createFile();
			break;
		case 2:	
			deleteFile();
			break;
		case 3:
			readFile();
			break;
		case 4:
			modifyFile();
			break;
		case 5: 
			showFiles();
			break;
		case 6: 
			// EXIT
			exit = true;
			break;
		default:
			cout << "\"" << command << "\" is not a valid command, please try again." << endl;
			break;
		}	
	}
}

void connectToServer()
{
	string ip, port;

//	cout << "Please enter the server IP: ";
//	cin >> ip;
	ip = "localhost";

	cout << "Please enter the port number: ";
	cin >> port;
	int portNo = 5020 + port[0] - '0';
	cout << portNo << endl;
	cout << "Please enter the clients port number: ";
	unsigned int clientPort[2];
	cin >> port;
	clientPort[0] = 5020 + port[0] - '0';

	// Connect to server
	string tmp[] = {"localhost", "localhost"};
	server = new Server(ip, tmp, clientPort, portNo);
}

void createFile()
{
	string title, content;

	cout << "title: " << endl;
	cin >> title;
	cout << "content: " << endl;
	cin >> content;

	server->newFile(title, content);
}

void deleteFile()
{
	string title, content;
	int id;

	cout << "id of the file: " << endl;
	cin >> id;

	server->deleteFile(id);
}

void readFile()
{
	int id;

	cout << "id of the file: " <<endl;
	cin >> id;
	
	File* f = server->readFile(id); 
	if (f)
		cout << f->get_content() << endl;
}

void modifyFile()
{
	int id;
	string title, content;
		
	cout << "id of the file: " << endl;
	cin >> id;
	cout << "title of the file: " << endl;
	cin >> title;
	cout << "content of the file: " << endl;
	cin >> content;
	server->updateFile(id, title, content);
}

void showFiles()
{
	server->listFiles();
}
