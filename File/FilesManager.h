#if not defined FILESMANAGER_H
#define FILESMANAGER_H
#include "File.h"
#include <stdexcept>
#include <map>

using namespace std;


class FilesManager {

	public:
		FilesManager() {}
		void add(File *file);
		
		/* DEBUG */
		void showAll();	
		
		void erase(int file_id);
		void erase(File *file);
		
		File* read(int id);

		void modify(int id, std::string title, std::string content);

	private:
		std::map<int,File *> files;

};


#endif

