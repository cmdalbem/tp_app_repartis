#include "createServer.h"


void connectToServer()
{
	string ip, port;

	cout << "Please enter the server IP: ";
	cin >> ip;

	cout << "Please enter the id (<10): ";
	cin >> port;
	int machineId = port[0] - '0';
	cout << "Please enter the port number: ";
	cin >> port;
	int portNo = 5020 + port[0] - '0';
	cout << portNo << endl;
	cout << "Enter the number of machines (<10)" << endl;
	cin >> port;
	int nbMachine = port[0] - '0';
	unsigned int clientPort[10];
	vector<string> client_ip;
	string tmp;
	for (int i = 0; i < nbMachine - 1; i++) {
		cout << "Please enter the clients ip adress: ";
		cin >> tmp; client_ip.push_back(tmp);
		cout << "Please enter the clients port number: ";
		cin >> port;
		clientPort[i] = 5020 + port[0] - '0';
	}
	
	cout << "The machine " << machineId << " on port ";
	cout << portNo << " talks to";
	for (int i = 0; i < nbMachine - 1; i++) {
		cout << " " << clientPort[i];
	} 
	cout << "." << endl;
	
	// Connect to server
	new Server(machineId, ip, client_ip, clientPort, portNo);
}
int main(void) {
	connectToServer();
}

