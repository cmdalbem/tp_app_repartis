#include <iostream>

#include "FilesManager.h"

void FilesManager::add(File *file) {
	files.insert(std::pair<int,File *>(file->get_id(), file));
}

void FilesManager::showAll() {
	map<int,File*>::iterator it;
	for (it=files.begin(); it != files.end(); ++it) {
		cout << "(" << it->first << ", " <<  it->second->show() << ") " << endl;
	}
}	

void FilesManager::erase(int file_id) {
	files.erase(file_id);
}

void FilesManager::erase(File *file) {
	files.erase(file->get_id());
}
File* FilesManager::read(int id) {
	File* res;
	
	try {
		res = files.at(id);
	} catch (const out_of_range& e) {
		cout << "This file doesn't exist";
		res = NULL;
	}

	return res;
}

void FilesManager::modify(int id, string title, string content) {
	File *res;
	try {
		res = files.at(id);
	} catch (const out_of_range& e) {
		cout << "This file doesn't exist" << endl;
	}
	res->set_title(title);
	res->set_content(content);
	
}	