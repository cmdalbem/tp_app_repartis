#include "File.h"
#include <sstream>

/* creating a serializable version of the file */
void File::to_JSON(string& output_data) 
{
	Json::Value data;
	data["id"] = this->id;
	data["title"] = this->title;
	data["content"] = this->content;

	Json::Value ownersNode;
	for (unsigned int i = 0; i < owners.size(); ++i)
		ownersNode.append((unsigned int) owners[i]);
	data["owners"] = ownersNode;
	
	Json::StyledWriter writer;
	output_data = writer.write(data);
}

/* recreating the file from its serializable version */
int File::parse_JSON(string json_data)
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
	for (unsigned int i = 0; i < data["owners"].size(); i++ )
		this->owners.push_back(data["owners"][i].asInt());

	return 0;
}
/*Debug function in order to see the content of a file */
string File::show() {
	string res;

	stringstream ss;
	ss << this->id;

	res = "id = " + ss.str() + ", title = " + this->title + ", content= " + this->content + ", owners= ";

	for (unsigned int i = 0; i < owners.size(); ++i){
		ss << owners[i];
		res += ss.str() + ", ";
	}
	
	return res;
}
