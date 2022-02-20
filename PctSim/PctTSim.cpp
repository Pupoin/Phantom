#include "json/reader.h"
#include "json/json.h"
#include "json/value.h"
#include "json/writer.h"

#include <vector>
#include <iomanip>
#include <fstream>
#include <iostream>
#include <exception>

#include "sim/hooks/PctActionInitialization.hh"
#include "sim/detector/PctDetectorConstruction.hh"
#include "sim/run/PctRunOpt.hh"
//#include "sim/TSensitiveDetector.hh"

#include "CmdParser.hh"
#include "Json2Geant4Macro.hh"
#include "FTFP_BERT.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "G4GDMLParser.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4UnitsTable.hh"
#include "G4Material.hh"
#include "G4Exception.hh"
#include "G4RunManagerFactory.hh"
//#include <unistd.h>
#include <strstream>
#include <iomanip>
#include "G4StateManager.hh"
#include "G4Threading.hh"
#include "G4ThreadPool.hh"
#include "G4SliceTimer.hh"
#include <stdio.h>

#define STATUS_MSG(INFO) std::cout<<std::setw(50)<<std::setfill('.')<<(INFO)<<std::endl
#define STATUS_MSG_OK(INFO) std::cout<<std::setw(50)<<std::setfill('.')<<(INFO)<<std::setfill(' ')<<std::setw(10)<<"[OK]"<<std::endl

int main(int argc, char **argv)
{
	cli::Parser cmdParser(argc, argv);
	cmdParser.set_required<std::string>("c", "setup", "default.json", "specify the options for this simulation.");
	cmdParser.set_optional<std::string>("m", "macro", "", "specify the macro file for geant4 simulaiton.");
	cmdParser.run_and_exit_if_error();


	std::string cfgFile = cmdParser.get<std::string>("c");
	std::string macFile = cmdParser.get<std::string>("m");

	std::ifstream ifs(cfgFile, std::ios::in);
	if (!ifs.good())
	{
		std::cerr << "Fail to open " << cfgFile << " to set up a simulation task." << std::endl;
		return EXIT_FAILURE;
	}

	Json::Value* root = new Json::Value();
	Json::CharReaderBuilder* builder = new Json::CharReaderBuilder();
	(*builder)["collectComments"] = false;
	JSONCPP_STRING errs;

	std::cout << std::setiosflags(std::ios::left);
	STATUS_MSG("[read options]");
	if (!parseFromStream(*builder, ifs, root, &errs))
	{
		std::cerr << "Fail to read " << cfgFile << " to set up a simulation task." << std::endl;
		ifs.close();
		return EXIT_FAILURE;
	}
	ifs.close();
	//std::cout << root << std::endl;
	STATUS_MSG_OK("[read options]");

	// Json::Value::Members key = root.getMemberNames();
	// Json::Value::Members::iterator keyit = key.begin();
	// Json::Value::Members::iterator keyend = key.end();
	// Json::Value val;
	// for (; keyit != keyend; ++keyit)
	//{
	//	std::cout << (*keyit) << " -> " << root[*keyit] << std::endl;
	// }
	

	// generate a geant4 macro file using the json file
	STATUS_MSG("[check GDML]");
	G4String gdmlFile = (*root)["geometry"]["gdml_file"].asString(); // hard-coded
	gdmlFile.strip(0,'"');
	FILE* fHnd(NULL);
	if ((fHnd=fopen(gdmlFile.c_str(), "r")) == NULL)
	{
		std::cerr << "Fail to access the GDML file #" << gdmlFile << std::endl;
		std::cerr << "please make sure it existed and readable." << std::endl;
		return EXIT_FAILURE;
	}
	fclose(fHnd);
	STATUS_MSG_OK("[check GDML]");

	G4SliceTimer* watch = new G4SliceTimer();
	watch->Start();
	STATUS_MSG("[setup detctor]");
	G4GDMLParser *parser = new G4GDMLParser();
	parser->Read(gdmlFile);
	G4VPhysicalVolume *pWorldVolume = parser->GetWorldVolume();
	if (!pWorldVolume)
	{
		std::cerr << "Fail to setup detector with the GDML file #" << gdmlFile << std::endl;
		std::cerr << "please make sure this GDML file valid." << std::endl;
		delete parser;
		return EXIT_FAILURE;
	}
	STATUS_MSG_OK("[setup detctor]");



	STATUS_MSG("[run initialization]");
	PctRunOpt::OptOutputFile = (*root)["output"].asString();
	G4int nThreads = (*root)["workers"].asInt();
	auto *runManager = G4RunManagerFactory::CreateRunManager();
	runManager->SetNumberOfThreads(nThreads>0 ? nThreads: 2);//根据配置文件设定
	runManager->SetUserInitialization(new FTFP_BERT);
	runManager->SetUserInitialization(new PctDetectorConstruction(pWorldVolume, parser));
	runManager->SetUserInitialization(new PctActionInitialization());
	runManager->Initialize();
	runManager->SetPrintProgress(3);
	STATUS_MSG_OK("[run initialization]");

	//return 0;

	STATUS_MSG("[simulation]");
	G4cout<<"load macro file #"<<macFile<<G4endl;
	if (macFile.empty())
	{
		G4UIExecutive *ui = new G4UIExecutive(argc, argv);
		ui->SessionStart();
		delete ui;
	}
	else
	{
		STATUS_MSG("[generate macro]");
		if (!json2macro(*root, macFile))
		{
			std::cerr << "Fail to prepare the geant4 macro file with " << cfgFile << std::endl;
			std::cerr << "please make sure you are granted to write to " << macFile << std::endl;
			return EXIT_FAILURE;
		}
		STATUS_MSG_OK("[generate macro]");
		// Get the pointer to the User Interface manager
		G4UImanager *UImanager = G4UImanager::GetUIpointer();
		G4String command = "/control/execute ";
		G4String fileName = macFile;
		UImanager->ApplyCommand(command + fileName);
	}
	STATUS_MSG_OK("[simulation]");

	watch->Stop();
	G4cout << "Time used #"<<watch->GetRealElapsed()<<" seconds"<<G4endl;
	delete watch;

	//G4StateManager *smgr = G4StateManager::GetStateManager();
	//G4cout << smgr->GetStateString(smgr->GetCurrentState()) << G4endl;

	G4cout <<"Main Thread ID =" <<G4Threading::G4GetThreadId() << G4endl;
	G4cout <<"Number of worker threads #" <<runManager->GetNumberOfThreads() << G4endl;
	G4cout <<"Number of available cores in this machine =" <<G4Threading::G4GetNumberOfCores() << G4endl;

	STATUS_MSG("[release resource]");
	// release memory
	delete root;
	delete builder;
	delete parser;
	delete runManager;
	STATUS_MSG_OK("[release resource]");


	return EXIT_SUCCESS;
}
