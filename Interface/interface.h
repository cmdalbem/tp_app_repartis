#ifndef _interface_h
#define _interface_h
#include <stdio.h>
#include <stdlib.h>
#include <string>
class Server;
class Connector;
using namespace std;
void *interfaceMain(void* context);

void connectToServer(Server *server);

void createFile(Server *server);

void deleteFile(Server *server);

void readFile(Server *server);

void modifyFile(Server *server);

void showFiles(Server *server);

string msg_new_file(string title, string content);
string msg_update_file(string title, string content, int file_id);
string msg_delete_file(int file_id);
string msg_read_file(int file_id);

#endif
