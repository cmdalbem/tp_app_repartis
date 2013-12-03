#include <iostream>
#include <vector>

using namespace std;

class Connector {
public:
	Connector();
	~Connector();

	void send(string ip, string msg);
	void broadcast(string msg);
	
	void addConnection(string ip);



private:
	vector<string> pairs;

}