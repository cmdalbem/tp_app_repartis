#define INTERFACE_H
#include <iostream>
#include <string>

#include "interface.h"
#include <Sender.h>
#include <ClientReceiver.h>
#include <Event.h>

using namespace std;

Sender sender(1000, "localhost");
ClientReceiver receiver;

void initialize() {
	receiver=ClientReceiver();
	receiver.serverMain(1001, 1337);		
	while (!sender.isConnected()) {
		sender.SenderMain("127.0.0.1",0);
		sleep(1);
	}
}


int main(void) {
	int command;
	initialize();

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
	string msg="";
	Event event=Event(CreateFile,msg);
	sender.update(event);
}

void deleteFile()
{
	string title, content;
	int id;

	cout << "id of the file: " << endl;
	cin >> id;
	string msg="";
	Event event=Event(DeleteFile,msg);
	sender.update(event);
}

void readFile()
{
	int id;

	cout << "id of the file: " <<endl;
	cin >> id;
	string msg="";
	Event event=Event(ReadFile,msg);
	sender.update(event);

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
	string msg="";
	Event event=Event(ModifyFile,msg);
	sender.update(event);
}

void showFiles()
{
	string msg="";
	Event event=Event(ShowFiles,msg);
	sender.update(event);
}
