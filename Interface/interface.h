#include <stdio.h>
#include <stdlib.h>
#include <string>

#include <Sender.h>

void connectToServer();

void createFile();

void deleteFile();

void readFile();

void modifyFile();

void showFiles();

string msg_new_file(string title, string content);
string msg_update_file(string title, string content, int file_id);
string msg_delete_file(int file_id);
string msg_read_file(int file_id);
