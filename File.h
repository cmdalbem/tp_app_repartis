#if not defined FICHIER_H
#define FICHIER_H
#include <iostream>
#include <jsoncpp/json.h>

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
	/*
	 * Handling JSON
	 */
	void to_JSON(std::string& output_data) 
	{
		Json::Value data;
		data["id"] = this->id;
		data["title"] = this->title;
		data["content"] = this->content;
		Json::StyledWriter writer;
		output_data = writer.write(data);
	}
	
	int parse_JSON(std::string json_data)
	{
		Json::Reader reader;
		Json::Value data;
		bool parsingSuccessful = reader.parse(json_data, data);
		if (!parsingSuccessful) {
			return 1;
		}
		this->id = data["id"].asInt();
		this->title = data["title"].asString();
		this->content = data["content"].asString();
		return 0;
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


