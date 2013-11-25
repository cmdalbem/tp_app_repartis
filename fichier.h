#if not defined FICHIER_H
#define FICHIER_H
#include <iostream>

class File {
	int id;
	std::string title;
	std::string content;
	public:
	
	File(int id, std::string title, std::string content)
	{
		this->id = id;
		this->title = title;
		this->content = content;
	}	


	/* 
	 * Getters and setters
	 */
	int get_id() {
		return this->id;
	}
	void set_id(int idf)
	{
		this->id = idf;
	}
	std::string get_title() {
		return this->title;
	}
	void set_title(std::string new_title)
	{
		this->title = new_title;
	}
	std::string get_content() {
		return this->content;
	}
	void set_content(std::string new_content)
	{
		this->content = new_content;
	}
};
#endif


