#define INTERFACE_H
#include <iostream>
#include <string>

#include <jsoncpp/json.h>
#include "interface.h"
#include <Connector.h>
#include <Server.h>

using namespace std;

void *interfaceMain(void *context) {
	//int command;
	Server *server = (Server *) context;

	//bool exit = false;
	sleep(10);
	string title = "test1";
	string content = "content_of_test1";
	string msg(msg_new_file(title,content));
	server->handleMessage(msg.c_str());
	return (void *)0;

	/*
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
				createFile(server);
				break;
			case 2:	
				deleteFile(server);
				break;
			case 3:
				readFile(server);
				break;
			case 4:
				modifyFile(server);
				break;
			case 5: 
				showFiles(server);
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
	*/	
}

void createFile(Server *server)
{
	string title, content;

	cout << "title: " << endl;
	cin >> title;
	cout << "content: " << endl;
	cin >> content;
	string msg=msg_new_file(title,content);;
	server->handleMessage(msg.c_str());
}

void deleteFile(Server *server)
{
	string title, content;
	int id;

	cout << "id of the file: " << endl;
	cin >> id;
	string msg=msg_delete_file(id);
	server->handleMessage(msg.c_str());
}

void readFile(Server *server)
{
	int id;

	cout << "id of the file: " <<endl;
	cin >> id;
	string msg=msg_read_file(id);
	server->handleMessage(msg.c_str());

}

void modifyFile(Server *server)
{
	int id;
	string title, content;
		
	cout << "id of the file: " << endl;
	cin >> id;
	cout << "title of the file: " << endl;
	cin >> title;
	cout << "content of the file: " << endl;
	cin >> content;
	string msg=msg_update_file(title,content,id);
	server->handleMessage(msg.c_str());
}

void showFiles(Server *server)
{
	string msg="";
}


string msg_new_file(string title, string content) {
	Json::Value data;
	
	data["type"] = "new_file";
	data["title"] = title;
	data["content"] = content;
	
	Json::StyledWriter writer;
	return writer.write(data);

}
string msg_update_file(string title, string content, int file_id) {
	Json::Value data;
	
	data["type"] = "update_file";
	data["title"] = title;
	data["content"] = content;
	data["file_id"] = file_id;
	
	Json::StyledWriter writer;
	return writer.write(data);
}
string msg_delete_file(int file_id) {
	Json::Value data;
	
	data["type"] = "delete_file";
	data["file_id"] = file_id;
	
	Json::StyledWriter writer;
	return writer.write(data);
}
string msg_read_file(int file_id) {
	Json::Value data;
	
	data["type"] = "read_file";
	data["file_id"] = file_id;
	
	Json::StyledWriter writer;
	return writer.write(data);
}
