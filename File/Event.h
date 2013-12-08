#ifndef EVENT_H
#define EVENT_H
#include <string>
#include <iostream>

enum Type {Ping,BetweenServer,CreateFile,DeleteFile,ReadFile,ModifyFile,ShowFiles};
class Event {
	public:
	Type getType() { return this->type; }
	void setType(Type type) { this->type = type; }
	
	std::string getMessage() { return this->message; }
	void setMessage(std::string message) { this->message = message; }
	
	Event(Type type, std::string message) 
	{
		this->type = type;
		this->message = message;
	}
	private:
	Type type;
	std::string message;
};

#endif

