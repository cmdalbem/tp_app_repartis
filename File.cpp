#include "File.h"
#include <sstream>
/*
void File::to_JSON(std::string& output_data) 
{
	Json::Value data;
	data["id"] = this->id;
	data["title"] = this->title;
	data["content"] = this->content;
	Json::StyledWriter writer;
	output_data = writer.write(data);
}
int File::parse_JSON(std::string json_data)
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
*/
std::string File::show() {
	std::string res;
	std::stringstream ss;
	ss << this->id;
	std::string idf = ss.str();
	res = "id = " + idf + ", title = " + this->title + ", content= " + this->content;
	return res;
}
