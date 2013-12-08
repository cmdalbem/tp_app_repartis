#include <iostream>
#include <string>
#include <Event.h>
#include "Connector.h"
#include "Server.h"
#include <cmath>
#include <fstream>

#include <jsoncpp/json.h>

using namespace std;

const string Server::configFile = "system.cfg";

void Server::configure() {
	lastFileId = 0;

	ifstream file;
	file.open(this->configFile.c_str());
	if (! file.is_open()) {
		file.open(("../" +this->configFile).c_str());
	}
	if (! file.is_open()) {
		printf("Error on configuration file %s opening\n", configFile.c_str());
		exit(0);
	}

	string tmp;
	char buffer[200];
	file.getline(buffer, 200);
	file >> nbMaxErrors;  
	file.getline(buffer, 200);
	file.getline(buffer, 200);
	file >> nbMaxClients;
} 

Server::Server(string ip) {
	configure();
	this->connector = Connector(this); 
	this->ip = ip;
	lastFileId = 0;
}	


Server::Server(int machineId, string ip, string listIpAdress[], unsigned int clientPortNo[], unsigned int portNo) {
	cout << "entering Server" << endl;
	configure();
	this->ip = ip;
	this->machineId = machineId;
	// do the complex server/client initializations here 
	this->connector = Connector(machineId, nbMaxClients, listIpAdress, clientPortNo, portNo, this);
	this->connector.initialize(nbMaxClients, listIpAdress, clientPortNo);
	cout << "quitting Server" << endl;
} 

Server::~Server() {	

}

void Server::ping() {
	cout << "Sending a ping" << endl;
	Event event = Event(Ping,"Ping");
	this->connector.senders[0]->threadedSender->update(event);
	cout << "Send the ping" << endl;
}

//////////////////////
// Files Management //
//////////////////////

// returns the integer concatenation of the machineId with the lastFileId
int Server::getNewFileId()
{
	#define COUNT_DIGITS(x) ((int) log10(x)+1)

	// If machineId is '127001' and lastFileId is '666', then newFileId => 127001666
	int newFileId = machineId * pow(10,COUNT_DIGITS(lastFileId)) + lastFileId;
	return newFileId;
}

//File* Server::newFile(string title, string content) {
void Server::newFile(string title, string content, vector<int> owners) {
	string buffer;
	string fileMsg;

	// Create new file
	File* f = new File(getNewFileId(), title, content, owners);
	fileMsg = msg_file_transfer(f);

	// Finds out who is alive
	connector.broadcast(msg_aliveQ());

	// Replicate it in the network
	string src;
	for (unsigned int i = 0; i < nbMaxErrors+1; ++i) {
		// Send the file for the first K+1 guys who answers
		connector.receive(&src,&buffer);
		connector.send(src,fileMsg);
	}

	// Add file locally
	manager.add(f);

	lastFileId++;
}

void Server::updateFile(int file_id, string title, string content, vector<int> owners) {
	string buffer;
	string src;

	// Asks who has the file
	connector.broadcast(msg_who_has(file_id));

	// Waits until someone answers
	File* f = new File(file_id, title, content, owners);
	string msg = msg_file_transfer(f);

	while (connector.receive(&src,&buffer))
		connector.send(src,msg);

	// Check if the file is available locally
	File* file = manager.read(file_id);
	if(file)
		manager.modify(file_id, title, content);

}

void Server::deleteFile(int file_id) {
	string msg;
		
	// Tells everyone that this file has to be deleted
	connector.broadcast(msg_del(file_id));

	// Check if the file is available locally
	File* file = manager.read(file_id);
	if(file)
		manager.erase(file_id);

}
	
File* Server::readFile(int file_id) {
	File* file = NULL;

	// Check if the file is available locally
	file = manager.read(file_id);
	if(file)
		return file;
	else {	
		string msg;

		// Asks who has the file
		connector.broadcast(msg_who_has(file_id));

		// Waits until someone answers
		string src;
		connector.receive(&src,&msg);

		// Asks the file for the first who answered
		connector.send(src,msg_file_req(file_id));

		// Receive the file
		connector.receive(&src,&msg);

		file = new File();
		file->parse_JSON(msg);

		return file;
	}

}

void Server::listFiles() {
	// TODO: how to retrieve a full list of files through servers?
	
	// temporary: just list local files for now
	manager.showAll();
}

void Server::restart() {
	// TODO: make it select only one machine to receive a given file from.
	
	// Tells everyone that you're coming back
	connector.broadcast(msg_reestart());

	// Receives files from people
	string src, buffer;
	while (connector.receive(&src,&buffer)) {
		File *f = new File(buffer);
		manager.add(f);
	}

}

void Server::handleMessage(char *msg) {
	cout<<"handleMessage: " << msg  << endl;
	ping();
	return;

	// TODO: identify who's the source of the message
	string srcIp;
	int srcId;

	Json::Reader reader;
	Json::Value data;

	// Parses the msg encoded in JSON
	string json_data(msg);
	bool parseOk = reader.parse(json_data, data);
	if(!parseOk) {
		cout << "ERROR can't handle message because I couldn't parse it's JSON.";
		return;
	}
	string msgType = data["type"].asString();

	// With the correct field extracted, we compare if with all possible msg types
	const int n = 12;
	string types[] = {"file_req","who_has","i_has","reestart",
					  "del","alive?","alive!","new_file","update_file",
					  "delete_file","read_file","file_transfer"};
	int whichType=-1;
	// Compare then all. If found correct type, puts it in "whichType"
	for (int i = 0; i < n; ++i) {
		if (types[i].compare(msgType) == 0) {
			whichType = i;
			break;
		}
	}

	cout << "Identified msg as being of type: " << whichType << endl;

	// Now handle each one properly
	switch(whichType) {
		////////////////////////////
		// SERVER/SERVER Messages //
		////////////////////////////

		case 0:
			// file_req
			{
				// 	Check if I have this file locally. If yes, sends it.
				int file_id = data["file_id"].asInt();
				File *f = manager.read(file_id);
				if(f)
					connector.send(srcIp, msg_file_transfer(f));
			}
			break;

		case 1:
			// who_has
			{
				// 	Check if I have this file locally. If yes, answers with a "i_has" msg.
				int file_id = data["file_id"].asInt();
				File *tmp = manager.read(file_id);
				if(tmp)
					connector.send(srcIp, msg_i_has(file_id));
			}

			break;

		case 2:
			// i_has
			break;

		case 3:
			// reestart
			{
				// Checks locally if I have any files that should be also owned by 'src'
				// If yes, send it to him.
				vector<File*> files = manager.getAll();
				for (unsigned int i = 0; i < files.size(); ++i) {
					for (unsigned int k = 0; k < files[i]->owners.size(); ++k) {
						if (files[i]->owners[k] == srcId) {
							connector.send(srcIp, msg_file_transfer(files[i]));
							break;
						}
					}
				}
			}

			break;

		case 4:
			// del
			break;

		case 5:
			// alive?

			connector.send(srcIp, msg_aliveA());

			break;

		case 6:
			// alive!
			break;


		////////////////////////////
		// CLIENT/SERVER Messages //
		////////////////////////////

		case 7: 
			// new_file
			{
				vector<int> owners;
				for (unsigned int i = 0; i < data["owners"].size(); ++i)
					owners.push_back(data["owners"][i].asInt());
				newFile(data["title"].asString(), data["content"].asString(), owners);
			}
			break;

		case 8:
			// update_file
			{
				vector<int> owners;
				for (unsigned int i = 0; i < data["owners"].size(); ++i)
					owners.push_back(data["owners"][i].asInt());
				updateFile(data["file_id"].asInt(), data["title"].asString(), data["content"].asString(), owners);
			}
			break;

		case 9:
			// delete_file
			deleteFile(data["file_id"].asInt());
			break;

		case 10:
			// read_file
			{
				File* f = readFile(data["file_id"].asInt());
				// TODO: do something with it (send it back to the Interface?)
			}
			break;


		//////////////////////
		// ALL/ALL Messages //
		//////////////////////

		case 11:
			// file_transfer
			// TODO: what to do???
			break;

		default:
			break;
	}

	return;	
}


string Server::msg_file_req(int file_id) {
	Json::Value data;
	
	data["type"] = "file_req";
	data["file_id"] = file_id;
	
	Json::StyledWriter writer;
	return writer.write(data);
}
string Server::msg_who_has(int file_id) {
	Json::Value data;
	
	data["type"] = "who_has";
	data["file_id"] = file_id;
	
	Json::StyledWriter writer;
	return writer.write(data);
}
string Server::msg_i_has(int file_id) {
	Json::Value data;
	
	data["type"] = "i_has";
	data["file_id"] = file_id;
	
	Json::StyledWriter writer;
	return writer.write(data);
}
string Server::msg_reestart() {
	Json::Value data;
	
	data["type"] = "reestart";
	
	Json::StyledWriter writer;
	return writer.write(data);
}
string Server::msg_del(int file_id) {
	Json::Value data;
	
	data["type"] = "del";
	data["file_id"] = file_id;
	
	Json::StyledWriter writer;
	return writer.write(data);
}
string Server::msg_aliveQ() {
	Json::Value data;
	
	data["type"] = "alive?";
	
	Json::StyledWriter writer;
	return writer.write(data);
}
string Server::msg_aliveA() {
	Json::Value data;
	
	data["type"] = "alive!";
	
	Json::StyledWriter writer;
	return writer.write(data);
}
string Server::msg_new_file(string title, string content) {
	Json::Value data;
	
	data["type"] = "new_file";
	data["title"] = title;
	data["content"] = content;
	
	Json::StyledWriter writer;
	return writer.write(data);

}
string Server::msg_update_file(string title, string content) {
	Json::Value data;
	
	data["type"] = "update_file";
	data["title"] = title;
	data["content"] = content;
	
	Json::StyledWriter writer;
	return writer.write(data);
}
string Server::msg_delete_file(int file_id) {
	Json::Value data;
	
	data["type"] = "delete_file";
	data["file_id"] = file_id;
	
	Json::StyledWriter writer;
	return writer.write(data);
}
string Server::msg_read_file(int file_id) {
	Json::Value data;
	
	data["type"] = "read_file";
	data["file_id"] = file_id;
	
	Json::StyledWriter writer;
	return writer.write(data);
}
string Server::msg_file_transfer(string fileJson) {
	Json::Value data;
	
	data["type"] = "file_transfer";
	data["fileJson"] = fileJson;
	
	Json::StyledWriter writer;
	return writer.write(data);
}
string Server::msg_file_transfer(File* f) {
	Json::Value data;

	string fileJson;
	f->to_JSON(fileJson);
	
	data["type"] = "file_transfer";
	data["fileJson"] = fileJson;
	
	Json::StyledWriter writer;
	return writer.write(data);
}
