#define INTERFACE_H
#include <iostream>
#include <string>

#include "interface.h"

using namespace std;


// Globals
Server server;
int lastId=0; //  has to be synchronized


int main(void) {
	int command;
	
	bool exit=false;
	
	while (not exit) {
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
			lastId++;
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
			break;
		}	
	}
}

void createFile()
{
	string title, content;

	cout << "title: " << endl;
	cin >> title;
	cout << "content: " << endl;
	cin >> content;
	server.newFile(lastId, title, content);
}

void deleteFile()
{
	string title, content;
	int id;

	cout << "id of the file: " << endl;
	cin >> id;
	server.deleteFile(id);
}

void readFile()
{
	int id;

	cout << "id of the file: " <<endl;
	cin >> id;
	
	File f = server.readFile(id); 
	cout << f.get_content() << endl;
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
	server.updateFile(id, title, content);
}

void showFiles()
{
	server.listFiles();
}