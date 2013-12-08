#if not defined FILESMANAGER_H
#define FILESMANAGER_H
#include "File.h"
#include <stdexcept>
#include <map>
#include <vector>

using namespace std;


class FilesManager {

	public:
		FilesManager() {}
		bool add(File *file);
		
		void showAll();	
		void erase(int file_id);
		void erase(File *file);
		File* read(int id);

		int countFiles();

		vector<File*> getAll();

		void modify(int id, std::string title, std::string content);

	private:
		std::map<int,File *> files;

};


#endif

