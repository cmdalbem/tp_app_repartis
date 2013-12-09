#include "createServer.h"
#include <sstream>
unsigned long long createMachineId(string ip, string port) {
	unsigned long long machineId;
	string machineTmp = string(ip);
	for (unsigned int i=0; i<3; i++) {
		int pos = machineTmp.find('.');
		machineTmp.erase(pos,1);
	}
	machineTmp += "502" + port;
	istringstream( machineTmp ) >> machineId;
	return machineId;
}
void connectToServer()
{
	string ip, port;

	cout << "Please enter the server IP: ";
	cin >> ip;

	cout << "Please enter the port number: (<10)";
	cin >> port;

	unsigned long long machineId = createMachineId(ip,port);

	int portNo = 5020 + port[0] - '0';
	cout << portNo << endl;
	cout << "Enter the number of machines (<10)" << endl;
	cin >> port;
	int nbMachine = port[0] - '0';
	unsigned int clientPort[10];
	vector<string> client_ip;
	string tmp;
	vector<unsigned long long> othersMachineId;
	for (int i = 0; i < nbMachine - 1; i++) {
		cout << "Please enter the clients ip adress: ";
		cin >> tmp; client_ip.push_back(tmp);
		cout << "Please enter the clients port number: ";
		cin >> port;
		clientPort[i] = 5020 + port[0] - '0';
		othersMachineId.push_back(createMachineId(tmp,port));
		
	}
	
	cout << "The machine " << machineId << " on port ";
	cout << portNo << " talks to";
	for (int i = 0; i < nbMachine - 1; i++) {
		cout << " " << clientPort[i];
	} 
	cout << "." << endl;
	
	// Connect to server
	Server *server = new Server(machineId, ip, client_ip, clientPort, portNo, othersMachineId);
	server->initialize(client_ip, clientPort);
	
}
int main(void) {
	connectToServer();
}

