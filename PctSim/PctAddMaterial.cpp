
#include <iostream>
#include "json/reader.h"
#include "json/json.h"
#include "json/value.h"
#include "json/writer.h"
#include "G4NistManager.hh"
#include "G4ios.hh"
#include <iomanip>
#include <fstream>
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4UnitsTable.hh"
#include "G4Material.hh"
#include "CmdParser.hh"
#include "G4Exception.hh"
#include <exception>

using namespace std;

int main(int argc, char** argv)
{
	cli::Parser cmdParser(argc, argv);
	cmdParser.set_required<std::string>("m1", "file1", "file1.json", "specify the 1st material table.");
	cmdParser.set_required<std::string>("m2", "file2", "file2.json", "specify the 2nd material table.");
	cmdParser.set_required<std::string>("o", "dump", "material.json", "a json file to be dumped, by default: material.json");

	cmdParser.run_and_exit_if_error();

	std::string file1 = cmdParser.get<std::string>("m1");
	std::string file2 = cmdParser.get<std::string>("m2");
	std::string dumpFile = cmdParser.get<std::string>("o");

	std::ifstream ifs1(file1, std::ios::in);
	std::ifstream ifs2(file2, std::ios::in);

	if(!ifs1.good()){
		std::cerr<<"Fail to open "<<file1<<" for reading."<<std::endl;
		return EXIT_FAILURE;
	}
	if(!ifs2.good()){
		std::cerr<<"Fail to open "<<file2<<" for reading."<<std::endl;
		ifs1.close();
		return EXIT_FAILURE;
	}
	ofstream ofs(dumpFile, ios::out);
	if(!ofs.good()){
		std::cerr<<"Fail to open "<<dumpFile<<" for writing."<<std::endl;
		ifs1.close();
		ifs2.close();
		return EXIT_FAILURE;
	}

	Json::Value root1;
	Json::CharReaderBuilder rbuilder;
	rbuilder["collectComments"] = true;
	JSONCPP_STRING errs;
	//std::cout<<"start to load json file1 #"<<file1<<std::endl;
	if (!parseFromStream(rbuilder, ifs1, &root1, &errs)) {
		std::cout << "Fail to read "<<file1<<", error message:"<< errs << std::endl;
		ifs1.close();
		ofs.close();
		return EXIT_FAILURE;
	}

	Json::Value root2;
	Json::CharReaderBuilder rbuilder2;
	rbuilder2["collectComments"] = true;

	//std::cout<<"start to load json file2 #"<<file2<<std::endl;
	if (!parseFromStream(rbuilder2, ifs2, &root2, &errs)) {
		std::cout << "Fail to read "<<file2<<", error message:"<< errs << std::endl;
		ifs1.close();
		ifs2.close();
		ofs.close();
		return EXIT_FAILURE;
	}

	Json::Value jvAll;
	Json::Value::ArrayIndex n1 = root1.size();
	for(Json::Value::ArrayIndex ix=0;ix<n1;++ix)
		jvAll.append(root1[ix]);
	Json::Value::ArrayIndex n2 = root2.size();
	for(Json::Value::ArrayIndex ix=0;ix<n2;++ix)
		jvAll.append(root2[ix]);

	Json::StreamWriterBuilder wbuilder;
	const std::unique_ptr<Json::StreamWriter> writer(wbuilder.newStreamWriter());
	writer->write(jvAll, &ofs);

	ofs.close();
	ifs2.close();
	ifs1.close();

	return EXIT_SUCCESS;
}
