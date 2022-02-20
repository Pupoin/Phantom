#include <iostream>
//#include "json/reader.h"
//#include "json/json.h"
//#include "json/value.h"
//#include "json/writer.h"
//#include "G4NistManager.hh"
//#include "G4ios.hh"
#include <iomanip>
#include <fstream>
//#include "G4SystemOfUnits.hh"
//#include "G4PhysicalConstants.hh"
//#include "G4UnitsTable.hh"
//#include "G4Material.hh"
#include "CmdParser.hh"
//#include "G4Exception.hh"
#include <exception>
#include "sim/io/PctRawReader.hh"
#include <vector>

using namespace std;
using namespace PCT;

int main(int argc, char **argv)
{
	cli::Parser cmdParser(argc, argv);
	cmdParser.set_required<std::string>("l", "list", "", "a raw file to be listed event by event.");
	// cmdParser.set_required<std::string>("o", "dump", "material.json", "a json file to be dumped, by default: material.json");

	cmdParser.run_and_exit_if_error();

	std::string aRawFile = cmdParser.get<std::string>("l");
	// std::string dumpFile = cmdParser.get<std::string>("o");

	PctRawReader *gFileReader = PctRawReader::Instance();
	bool isFileOK = gFileReader->open(aRawFile.c_str());

	if (!isFileOK)
	{
		std::cerr << "Fail to open " << aRawFile << " for reading." << std::endl;
		return EXIT_FAILURE;
	}

	// Json::Value jvID;
	// Json::Value jvMaterial;
	// Json::Value jvMaterials;
	// read file, please refer to TEventAction::EndOfEventAction
	//int evt_header[2];
	//std::vector<ParticleInfo> ptl_info;
	//std::vector<Coor3D> ptl_pos;

	std::cout<<setiosflags(std::ios::left)<<setprecision(2)<<setiosflags(ios::fixed);
	std::cout<<setw(10)<<"RUN_ID"
		<<setw(10)<<"EVENT_ID"
		<<setw(10)<<"NTRACKS"<<std::endl;

	PCT::READER::PctEvent* evt = NULL;
	while ((evt = gFileReader->next()))
	{
		std::cout << (*evt) << std::endl;
	}
	gFileReader->close();

	return EXIT_SUCCESS;
}
