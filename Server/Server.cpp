#include <iostream>
#include <string>
#include <Event.h>
#include "Connector.h"
#include "Server.h"
#include <cmath>
#include <fstream>
#include <sstream>
#include <interface.h>

#include <jsoncpp/json.h>

using namespace std;

const string Server::configFile = "system.cfg";

void Server::configure() {
	// Initialize the msgsBuffer mutex
	pthread_mutex_init(&msgsBufferMutex, NULL);
	pthread_cond_init(&msgsBufferCond, NULL);

	// Initialize unique file ID
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


Server::Server(unsigned long long machineId, string ip, vector<string>& listIpAdress, unsigned int clientPortNo[], unsigned int portNo, vector<unsigned long long>& othersMachineId) {
	cout << "entering Server" << endl;
	configure();
	this->ip = ip;
	this->machineId = machineId;
	// Initialize Connector, which will initialize all Senders and Receivers necessary
	cout << "entering Connector" << endl;
	this->connector = Connector(machineId, nbMaxClients, listIpAdress, clientPortNo, portNo, this, othersMachineId);
	cout << "quitting Server" << endl;
} 

void Server::initialize(vector<string> listIpAdress, unsigned int clientPortNo[]) {
	this->connector.initialize(nbMaxClients, listIpAdress, clientPortNo,this);
}
Server::~Server() {	

}

void Server::ping() {
	cout << "ping not implem" << endl;
	return;
}

//////////////////////
// Files Management //
//////////////////////

// Returns the integer concatenation of the machineId with the lastFileId
// If machineId is '127001' and lastFileId is '666', then newFileId => 127001666
int Server::getNewFileId()
{
	#define COUNT_DIGITS(x) ((int) log10(x)+1)

	int newFileId = machineId * pow(10,COUNT_DIGITS(lastFileId)) + lastFileId;
	
	return newFileId;
}

void *newFile(void *arguments) {
	// Parse arguments
	struct newFile_args *args = (struct newFile_args *) arguments;
	Server *me = args->server;
	string title = args->title;
	string content = args->content;
	vector<unsigned long long int> owners = args->owners;

	MessageStoreNode buffer;

	// Build new file
	File* f = new File(me->getNewFileId(), title, content, owners);
	me->incrementLastFileId();
	f->owners.push_back(me->getId()); // adds myself to list of owners

	// Finds out who is alive
	me->connector.broadcast(me->msg_aliveQ());
	
	// Collects K ids of who replies
	for (unsigned int i = 0; i < me->nbMaxErrors; ++i) {
		while(!me->retrieveMessage(&buffer,"alive!"));
		f->owners.push_back(buffer.src);
	}
	
	// Converts file to JSON, now with the list of K+1 owners
	string fileMsg = me->msg_add(f);
	
	// Sends the file to those guys
	for (unsigned int i = 1; i < me->nbMaxErrors+1; ++i)
		me->connector.send(owners[i],fileMsg);

	// Adds file to local store
	me->manager.add(f);

	return 0;
}

void *updateFile(void *arguments) {
	// Parse arguments
	struct updateFile_args *args = (struct updateFile_args *) arguments;
	Server *me = args->server;
	int file_id = args->file_id;
	string title = args->title;
	string content = args->content;
	vector<unsigned long long int> owners = args->owners;

	// Asks who has the file
	me->connector.broadcast(me->msg_who_has(file_id));

	// Send the file to everyone that has the file
	File* f = new File(file_id, title, content, owners);
	string msg = me->msg_add(f);

	MessageStoreNode buffer;
    stringstream ss;
    ss << file_id;
	while(me->retrieveMessage(&buffer,string("i_has"),ss.str()))
		me->connector.send(buffer.src, msg);

	// Check if the file is available locally. If it is, update it
	File* file = me->manager.read(file_id);
	if(file)
		me->manager.modify(file_id, title, content);

	return 0;
}

void *deleteFile(void* arguments) {
	// Parse argumeasents
	struct file_args *args = (struct file_args *)arguments;
	Server *me = args->server;
	int file_id = args->file_id;

	// Tells everyone that this file has to be deleted
	me->connector.broadcast(me->msg_del(file_id));

	// Check if the file is available locally. If yes, delete it too.
	File* file = me->manager.read(file_id);
	if(file)
		me->manager.erase(file_id);

	return 0;
}
	
void *readFile(void* arguments) {
	// Parse arguments
	struct file_args *args = (struct file_args *) arguments;
	Server *me = args->server;
	int file_id = args->file_id;

	File* file = NULL;

	// Check if the file is available locally
	file = me->manager.read(file_id);
	if(!file) {
		// Asks everyone who has the file
		me->connector.broadcast(me->msg_who_has(file_id));

		MessageStoreNode buffer;
		stringstream ss;
		ss << file_id;
		
		// Waits until someone reply
		while(!me->retrieveMessage(&buffer,string("i_has"),ss.str()));

		// Asks the file for the first who replied
		me->connector.send(buffer.src, me->msg_file_req(file_id));

		// Receive the file
		while(!me->retrieveMessage(&buffer,string("file_transfer")));

		file = new File();
		file->parse_JSON(buffer.content);
	}

	// TODO: send the file to Interface (has to remember his IP...)

	return 0;
}

void *listFiles(void* arg) {
	// Parse arguments
	//Server *me = (Server*) arg;

	// TODO: how to retrieve a full list of files through servers?

	// TODO: send the files to Interface (has to remember his IP...)	

	return 0;
}

void *restart(void* arg) {
	// Parse arguments
	Server *me = (Server*) arg;

	// Tells everyone that you're coming back
	me->connector.broadcast(me->msg_restart());

	// Receives files from people
	MessageStoreNode buffer;
	while (me->retrieveMessage(&buffer,"file_transfer")) {
		File *f = new File(buffer.content);
		me->manager.add(f);
	}

	// Nice, now we're back! :)

	return 0;
}


/////////////////////////////
// Message Buffer managing //
/////////////////////////////

// Pushes a message to msgsBuffer with empty content
void Server::pushMessage(int src, string msg) {
	pushMessage(src,msg,"");
}

// Pushes a message to msgsBuffer
void Server::pushMessage(int src, string msg, string content) {
	// The mutex makes sure we are not reading and writing at the same time.
	// It can really happen, since each running algorithm is in fact a thread.
	pthread_mutex_lock(&msgsBufferMutex);
		bool wasEmpty = msgsBuffer.empty();
		msgsBuffer.push_back({src,msg,content});
		if(wasEmpty) {
			// Wakes up everybody that was sleeping waiting for messages
			pthread_cond_signal(&msgsBufferCond);
		}
	pthread_mutex_unlock(&msgsBufferMutex);
}

// Searches for a message with the desired type. If found, pop it and returns it.
bool Server::retrieveMessage(MessageStoreNode *ret, string type) {
	pthread_mutex_lock(&msgsBufferMutex);
		if(msgsBuffer.empty()) {
			// If queue is empty then releases the mutex and sleep until it's not anymore
			pthread_cond_wait(&msgsBufferCond,&msgsBufferMutex);
		}

		for (unsigned int i = 0; i < msgsBuffer.size(); ++i) {
			if (msgsBuffer[i].type.compare(type) == 0) {
				*ret = msgsBuffer[i];
				msgsBuffer.erase(msgsBuffer.begin()+i);
				pthread_mutex_unlock(&msgsBufferMutex);
				return true;
			}
		}
	pthread_mutex_unlock(&msgsBufferMutex);
	return false;
}

// Searches for a message with the desired type and desired content. If found, pop it and returns it.
bool Server::retrieveMessage(MessageStoreNode *ret, string type, string content) {
	pthread_mutex_lock(&msgsBufferMutex);
		if(msgsBuffer.empty()) {
			// If queue is empty then releases the mutex and sleep until it's not anymore
			pthread_cond_wait(&msgsBufferCond,&msgsBufferMutex);
		}
		for (unsigned int i = 0; i < msgsBuffer.size(); ++i) {
			if (msgsBuffer[i].type.compare(type) == 0 && msgsBuffer[i].content.compare(content)==0) {
				*ret = msgsBuffer[i];
				msgsBuffer.erase(msgsBuffer.begin()+i);
				pthread_mutex_unlock(&msgsBufferMutex);
				return true;
			}
		}
	pthread_mutex_unlock(&msgsBufferMutex);
	return false;
}

// Searches for a message with the desired type and desired source. If found, pop it and returns it.
bool Server::retrieveMessage(MessageStoreNode *ret, int src, string type) {
	pthread_mutex_lock(&msgsBufferMutex);
		if(msgsBuffer.empty()) {
			// If queue is empty then releases the mutex and sleep until it's not anymore
			pthread_cond_wait(&msgsBufferCond,&msgsBufferMutex);
		}
		for (unsigned int i = 0; i < msgsBuffer.size(); ++i) {
			if (msgsBuffer[i].type.compare(type) == 0 && msgsBuffer[i].src==src) {
				*ret = msgsBuffer[i];
				msgsBuffer.erase(msgsBuffer.begin()+i);
				pthread_mutex_unlock(&msgsBufferMutex);
				return true;
			}
		}
	pthread_mutex_unlock(&msgsBufferMutex);
	return false;
}


//////////////////////
// Message Handling //
//////////////////////

// Parsse and handles messages received by the Server. They can be sent by other Servers or
//   by an Interface (Client).
//   - Messages from the Interface launches threads that will execute the protocols for creating,
//     reading, ... files.
//   - Some messages sent by Servers my require immediate action (like "alive?", which just
//     replies "alive!"). Others (like "alive!") will be buffered in msgsBuffer, which stores
//     messages to be handled by the protocols' threads.
// For more information on the meaning of each message please refer to the Documentation.
void Server::handleMessage(const char *msg) {
	cout<<"handleMessage: " << msg  << endl;
	if (strcmp(msg,"")==0 || strcmp(msg," ")==0) 
		return;

	// TODO: identify who's the source of the message
	string srcIp;
	unsigned long long srcId;

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
	const int n = 13;
	string types[] = {"file_req","who_has","i_has","restart",
					  "del","alive?","alive!","add","new_file","update_file",
					  "delete_file","read_file","file_transfer"};
	// Compare then all. If found correct type, puts it in "whichType"
	int whichType=-1;
	for (int i = 0; i < n; ++i) {
		if (types[i].compare(msgType) == 0) {
			whichType = i;
			break;
		}
	}

	cout << "Identified msg as being of type: " << whichType << endl;

	// Now handle each type properly
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
					connector.send(srcId, msg_file_transfer(f));
			}
			break;

		case 1:
			// who_has
			{
				// 	Check if I have this file locally. If yes, replies with a "i_has" msg.
				int file_id = data["file_id"].asInt();
				if(manager.read(file_id))
					connector.send(srcId, msg_i_has(file_id));
			}

			break;

		case 2:
			// i_has
			// Push into the msgs buffer
			pushMessage(srcId,data["type"].asString(),data["file_id"].asString());
			break;

		case 3:
			// restart
			{
				// Checks locally if I have any files that should be also owned by 'src'
				// If yes, send it to him.
				vector<File*> files = manager.getAll();
				for (unsigned int i = 0; i < files.size(); ++i) {
					for (unsigned int k = 0; k < files[i]->owners.size(); ++k) {
						if (files[i]->owners[k] == srcId) {
							connector.send(srcId, msg_file_transfer(files[i]));
							break;
						}
					}
				}
			}

			break;

		case 4:
			// del
			{
				// 	Check if I have this file locally. If yes, erases it.
				int file_id = data["file_id"].asInt();
				File *f = manager.read(file_id);
				if(f)
					manager.erase(file_id);
			}
			break;

		case 5:
			// alive?
			connector.send(srcId, msg_aliveA());
			break;

		case 6:
			// alive!
			// Push into the msgs buffer
			pushMessage(srcId,data["type"].asString());
			break;

		case 7:
			// add
			{
				Json::Value fileData;
				string fileJson = data["fileJson"].asString();
				if (reader.parse(fileJson, fileData)) {
					// Check if already had the file. If yes, update the value. If not, add it
					int file_id = fileData["file_id"].asInt();
					File *f = manager.read(file_id);
					if(f)
						manager.modify(file_id,fileData["title"].asString(), fileData["content"].asString());
					else
						manager.add(new File(fileJson));
				}				
			}
			break;


		////////////////////////////
		// CLIENT/SERVER Messages //
		////////////////////////////

		case 8: 
			// new_file
			{
				cout << "received message to create a new file!" << endl;
				vector<unsigned long long int> owners;
				for (unsigned int i = 0; i < data["owners"].size(); ++i)
					owners.push_back(data["owners"][i].asInt());
				
				// Launch thread that will run the protocol algorithm
				pthread_t t;
				struct newFile_args args = {
					this,
					data["title"].asString(),
					data["content"].asString(),
					owners
				};
				pthread_create(&t, NULL, &newFile, (void*)&args);

				//newFile(data["title"].asString(), data["content"].asString(), owners);
			}
			break;

		case 9:
			// update_file
			{
				vector<unsigned long long int> owners;
				for (unsigned int i = 0; i < data["owners"].size(); ++i)
					owners.push_back(data["owners"][i].asInt());

				// Launch thread that will run the protocol algorithm
				pthread_t t;
				struct updateFile_args args = {
					this,
					data["file_id"].asInt(),
					data["title"].asString(),
					data["content"].asString(),
					owners
				};
				pthread_create(&t, NULL, &updateFile, (void*)&args);
			}
			break;

		case 10:
			// delete_file
			{
				// Launch thread that will run the protocol algorithm
				pthread_t t;
				struct file_args args = {
					this,
					data["file_id"].asInt()
				};
				pthread_create(&t, NULL, &deleteFile, (void*)&args);
			}
			break;

		case 11:
			// read_file
			{
				// Launch thread that will run the protocol algorithm
				pthread_t t;
				struct file_args args = {
					this,
					data["file_id"].asInt()
				};
				pthread_create(&t, NULL, &readFile, (void*)&args);
			}
			break;


		//////////////////////
		// ALL/ALL Messages //
		//////////////////////

		case 12:
			// file_transfer
			// Push into the msgs buffer for later handling
			pushMessage(srcId, data["type"].asString(), data["fileJson"].asString());
			break;

		default:
			break;
	}

	return;	
}

////////////////////////////////////////////
// Helper functions for creating messages //
////////////////////////////////////////////

// Returns a string in JSON format for each kind of message, with the arguments
//   properly encoded.

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
string Server::msg_restart() {
	Json::Value data;
	
	data["type"] = "restart";
	
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
string Server::msg_add(string fileJson) {
	Json::Value data;
	
	data["type"] = "add";
	data["fileJson"] = fileJson;
	
	Json::StyledWriter writer;
	return writer.write(data);
}
string Server::msg_add(File* f) {
	Json::Value data;

	string fileJson;
	f->to_JSON(fileJson);
	
	data["type"] = "add";
	data["fileJson"] = fileJson;
	
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
