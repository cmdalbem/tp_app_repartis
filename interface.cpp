#define INTERFACE_H
#include <iostream>
#include <string>
#include "FilesManager.h"

int main(void) {
	int command;
	FilesManager manager;
	int id=0; //  has to be synchronized
	int exit=0;
	while (!exit) {
		std::cout << "(1) to create a file\n"
			"(2) to delete a file\n"
			"(3) to read a file\n"
			"(4) to modify a file\n"
			"(5) show all files\n"
			"(6) to exit\n";
		std::cin >> command;
		std::string title;
		std::string content;
		int idRetrieved;
		switch(command) {
		case 1:
			std::cout << "title: " << std::endl;
			std::cin >> title;
			std::cout << "content: " << std::endl;
			std::cin >> content;
			manager.add(new File(id, title, content));
			id++;
			break;
		case 2:	
			std::cout << "id of the file: " << std::endl;
			std::cin >> idRetrieved;
			manager.erase(idRetrieved);
			break;
		case 3:
			std::cout << "id of the file: " <<std::endl;
			std::cin >> idRetrieved;
			std::cout << manager.show(idRetrieved) << std::endl;
			break;
		case 4:
			break;
		case 5: 
			manager.showAll();
			break;

		case 6: 
			exit=1;
			break;
		default:
			break;
		}	
	}
}


