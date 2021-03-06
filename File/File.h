#if not defined FILE_H
#define FILE_H
#include <jsoncpp/json.h>
#include <string>
#include <iostream>
#include <vector>

using namespace std;

class File {
	
public:
	File() {}

	File(string json)
	{
		parse_JSON(json);
	}

	File(int id, string title, string content, vector<unsigned long long int> owners)
	{
		this->id = id;
		this->title = title;
		this->content = content;
		this->owners = owners;
	}

	// Attributes
	int id;
	string title;
	string content;
	vector<unsigned long long int> owners;

	/* debug */
	string show();

	/*
	 * Handling JSON if needed
	 */
	void to_JSON(string& output_data);
	int parse_JSON(string json_data);
};
#endif


