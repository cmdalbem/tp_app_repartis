#if not defined FILE_H
#define FILE_H
//#include <jsoncpp/json.h>
#include <string>
#include <iostream>
class File {
	int id;
	std::string title;
	std::string content;
	public:
	
	File() {}

	File(int id, std::string title, std::string content)
	{
		this->id = id;
		this->title = title;
		this->content = content;
	}	
	/* debug */
	std::string show();

	/*
	 * Handling JSON if needed
	 */
	void to_JSON(std::string& output_data);
	int parse_JSON(std::string json_data);

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


