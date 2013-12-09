#include "createServer.h"
#include <sstream>
/*
	creation of an unique id for the machine in the distributed system
	ip is this ip adress of the machine
	port is the port where it is listening
*/
unsigned long long createMachineId(string ip,string port) {
	/* We just concatanate the ip and the port in an integer */
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
	/* getting the configuration of the server */
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
	/*getting to know the whole network of machines*/
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
	/* initializing the Sender (how we are going to communicate with the other servers) */
	server->initialize(client_ip, clientPort);
	
}
int main(void) {
	connectToServer();
}

