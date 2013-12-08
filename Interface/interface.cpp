#define INTERFACE_H
#include <iostream>
#include <string>

#include "interface.h"

using namespace std;


int main(void) {
	int command;

	bool exit = false;

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

void createFile()
{
	string title, content;

	cout << "title: " << endl;
	cin >> title;
	cout << "content: " << endl;
	cin >> content;

}

void deleteFile()
{
	string title, content;
	int id;

	cout << "id of the file: " << endl;
	cin >> id;

}

void readFile()
{
	int id;

	cout << "id of the file: " <<endl;
	cin >> id;

}

void modifyFile()
{
	int id, nowners;
	string title, content;

	cout << "id of the file: " << endl;
	cin >> id;
	cout << "title of the file: " << endl;
	cin >> title;
	cout << "content of the file: " << endl;
	cin >> content;
}

void showFiles()
{
}
