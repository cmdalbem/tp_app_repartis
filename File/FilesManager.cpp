#include <iostream>

#include "FilesManager.h"

bool FilesManager::add(File *file) {
	pair<map<int,File*>::iterator,bool> ret = files.insert(pair<int,File *>(file->id, file));
	// Returns if addition succeeded or if file_id already existed
	return ret.second;
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
	files.erase(file->id);
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
	res->title = title;
	res->content = content;
	
}	

int FilesManager::countFiles()
{
	return files.size();
}

vector<File*> FilesManager::getAll()
{	
	vector<File*> ret;
	for (map<int,File*>::iterator it = files.begin(); it != files.end(); ++it) 
		ret.push_back(it->second);

	return ret;
}