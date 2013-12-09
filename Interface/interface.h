#ifndef _interface_h
#define _interface_h
#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace std;

class Server;
class Connector;

void *interfaceMain(void* context);

void createFile(Server *server);

void deleteFile(Server *server);

void readFile(Server *server);

void modifyFile(Server *server);

void showFiles(Server *server);

// Returns a string in JSON format for each kind of message, with the arguments
//   properly encoded.
string msg_new_file(string title, string content);
string msg_update_file(string title, string content, int file_id);
string msg_delete_file(int file_id);
string msg_read_file(int file_id);

#endif
