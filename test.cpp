#include "File.h"
#include <vector>
#include <stdint.h>
#include "FilesManager.h"

int main(void)
{
	FilesManager manager;
	manager.add(new File(1,"test","test_content"));
	manager.add(new File(2,"test","test_content"));
	manager.add(new File(3,"test_a_eliminer","test_content"));
	manager.show();
	manager.erase(3);
	manager.show();
	
}
