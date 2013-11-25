#include "fichier.h"

int main(void) 
{
	File *f = new File(1,"titre","contenu");
	std::cout << "( id= " <<f->get_id() << ", title= " << f->get_title() << ", content= "<< f->get_content() << " )"<<std::endl;

}
