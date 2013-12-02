#if not defined FILESMANAGER_H
#define FILESMANAGER_H
#include "File.h"
#include <map>
class FilesManager {
	std::map<int,File *> files;
	public:
	FilesManager() {}
	void add(File *file) {
		files.insert(std::pair<int,File *>(file->get_id(), file));
	}
	/* DEBUG */
	void show() {
		std::map<int,File*>::iterator it;
		for (it=files.begin(); it != files.end(); ++it) {
			std::cout << "(" << it->first << ", " <<  it->second->show() << ") " << std::endl;
		}
	}	
	void erase(int file_id) {
		files.erase(file_id);
	}
	void erase(File *file) {
		files.erase(file->get_id());
	}
	
};


#endif

