
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
	cmdParser.set_optional<std::string>("l", "list", "", "list the element table, an optional parameter");
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
	const std::vector<G4String> &eleNames = nistMgr->GetNistElementNames();

	// G4cout << "Material database # " << eleNames.size() << " types" << G4endl;
	// G4cout << "Dump material into json file." << G4endl;
	// G4cout << std::setw(5) << "ID"
	//        << std::setw(35) << "Name"
	//        << G4endl;

	for (size_t i = 0; i < eleNames.size(); ++i)
	{
		if(i>=101) break;
		G4cout << std::setw(5) << i
			<< std::setw(35) << eleNames[i] << G4endl;
		G4Element *theM = nistMgr->FindOrBuildElement(eleNames[i]);
		if (theM)
		{
			// jvMaterial["ID"] = i;
			jvMaterial["Symbol"] = theM->GetSymbol();
			jvMaterial["AtomIndex"] = theM->GetZ();
			jvMaterial["AtomMass"] = theM->GetAtomicMassAmu();
			jvMaterial["Coulomb"] = theM->GetfCoulomb();
			jvMaterial["MeanExcitationEnergy"] = theM->GetIonisation()->GetMeanExcitationEnergy();
			jvID["Name"] = eleNames[i];
			// jvID["Name"] = eleames[i].substr(3, matNames.size() - 3);
			jvID["Property"] = jvMaterial;
			jvMaterials.append(jvMaterial);
		}
	}
	Json::StreamWriterBuilder builder;
	const std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
	writer->write(jvMaterials, &ofs);
	ofs.flush();
	ofs.close();

	return EXIT_SUCCESS;
}
