
#pragma once

#include "json/reader.h"
#include "json/json.h"
#include "json/value.h"
#include "json/writer.h"
#include <fstream>


class PctJsonManager
{
	template <typename T> 
		void bind(const std::string& key, T& value){
		}

	bool ReadFile(const std::string& cFileName)
	{
		std::ifstream ifs(cFileName, std::ios::in);
		std::cout << std::setiosflags(std::ios::left);
		if (!ifs.good())
		{
			std::cerr << "Fail to open " << cFileName<< " to set up a simulation task." << std::endl;
			return false;
		}

		if(reader == NULL)
			reader = new Json::CharReaderBuilder();
		(*reader)["collectComments"] = false;
		JSONCPP_STRING errs;

		if (!parseFromStream(*reader, ifs, root, &errs))
		{
			std::cerr << "Fail to read " << cFileName << " to set up a simulation task." << std::endl;
			ifs.close();
			return false;
		}
		ifs.close();
		return true;
	}

	void write();
	private:

	PctJsonManager(): root(new Json::Value()), reader(NULL){
	}
	Json::Value* root;
	Json::CharReaderBuilder* reader;
};
