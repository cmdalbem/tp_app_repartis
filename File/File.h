#if not defined FILE_H
#define FILE_H
#include <jsoncpp/json.h>
#include <string>
#include <iostream>

using namespace std;

class File {
	int id;
	string title;
	string content;
	public:
	
	File() {}

	File(int id, string title, string content)
	{
		this->id = id;
		this->title = title;
		this->content = content;
	}	
	/* debug */
	string show();

	/*
	 * Handling JSON if needed
	 */
	void to_JSON(string& output_data);
	int parse_JSON(string json_data);

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
	string get_title() {
		return this->title;
	}
	void set_title(string new_title)
	{
		this->title = new_title;
	}
	string get_content() {
		return this->content;
	}
	void set_content(string new_content)
	{
		this->content = new_content;
	}
};
#endif


