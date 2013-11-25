#include "File.h"

int main(void) 
{
	File *f = new File(1,"titre","contenu");
	std::cout << "Test Fichier sans JSON" << std::endl;
	std::cout << "( id= " <<f->get_id() << ", title= " << f->get_title() << ", content= "<< f->get_content() << " )"<<std::endl;
	std::cout << "Test Fichier avec JSON" << std::endl;
	File *f2 = new File(2, "titre2","contenu2");
	std::string data;
	f2->to_JSON(data);
	File *f3 = new File();
	f3->parse_JSON(data);
	
	std::cout << "( id= " <<f3->get_id() << ", title= " << f3->get_title() << ", content= "<< f3->get_content() << " )"<<std::endl;
}
