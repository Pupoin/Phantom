
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
	cmdParser.set_optional<std::string>("l", "list", "", "list all materials, an optional parameter.");
	cmdParser.set_required<std::string>("o", "dump", "material.json", "a json file to be dumped, by default: material.json");

	cmdParser.run_and_exit_if_error();

	std::string choice = cmdParser.get<std::string>("l");
	std::string dumpFile = cmdParser.get<std::string>("o");

	ofstream ofs(dumpFile, ios::out);
	if(!ofs.good()){
		std::cerr<<"Fail to open "<<dumpFile<<" for dumping"<<std::endl;
		return EXIT_FAILURE;
	}


	Json::Value jvID;
	Json::Value jvMaterial;
	Json::Value jvMaterials;

	G4NistManager *nistMgr = G4NistManager::Instance();
	const std::vector<G4String> &matNames = nistMgr->GetNistMaterialNames();
	// G4cout << "Material database # " << matNames.size() << " types" << G4endl;
	// G4cout << "Dump material into json file." << G4endl;
	// G4cout << std::setw(5) << "ID"
	//        << std::setw(35) << "Name"
	//        << G4endl;

	for (size_t i = 0; i < matNames.size(); ++i)
	{
		G4cout << std::setw(5) << i
			<< std::setw(35) << matNames[i] << G4endl;
		G4Material *theM = nistMgr->FindOrBuildMaterial(matNames[i]);
		if (theM)
		{
			// jvMaterial["ID"] = i;
			// jvMaterial["AtomIndex"] = theM->GetZ();
			// jvMaterial["AtomMass"] = theM->GetA();
			jvMaterial["Density"] = G4String(G4BestUnit(theM->GetDensity(), "Volumic Mass"));
			jvMaterial["Pressure"] = G4String(G4BestUnit(theM->GetPressure(), "Pressure"));
			jvMaterial["NElements"] = Json::UInt64(theM->GetNumberOfElements());
			jvMaterial["RadationLength"] = G4String(G4BestUnit(theM->GetRadlen(), "Length"));
			jvMaterial["ChemicalFormula"] = theM->GetChemicalFormula();
			jvMaterial["NuclearInteractionLength"] = G4String(G4BestUnit(theM->GetNuclearInterLength(), "Length"));
			jvID["G4ID"] = matNames[i];
			jvID["Name"] = matNames[i].substr(3, matNames.size() - 3);
			jvID["Property"] = jvMaterial;
			jvMaterials.append(jvID);
		}
	}
	Json::StreamWriterBuilder builder;
	const std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
	writer->write(jvMaterials, &ofs);
	ofs.flush();
	ofs.close();

	return EXIT_SUCCESS;
}
