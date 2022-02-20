
#include <iostream>
#include "json/reader.h"
#include "json/json.h"
#include "json/value.h"
#include "json/writer.h"
#include "G4ParticleTable.hh"
#include "G4ios.hh"
#include <iomanip>
#include <fstream>
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4UnitsTable.hh"
#include "CmdParser.hh"
#include "G4Exception.hh"
#include <exception>
#include "G4RunManager.hh"
#include "G4VModularPhysicsList.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4OpticalPhysics.hh"
#include "FTFP_BERT.hh"

using namespace std;

int main(int argc, char** argv)
{
    cli::Parser cmdParser(argc, argv);
	cmdParser.set_optional<std::string>("l", "list", "", "list all paritcles");
	cmdParser.set_required<std::string>("o", "dump", "ptl.json", "a json file to be dumped, by default: ptl.json");

    cmdParser.run_and_exit_if_error();

    //std::string choice = cmdParser.get<std::string>("l");
    std::string dumpFile = cmdParser.get<std::string>("o");

    ofstream ofs(dumpFile, ios::out);
    if(!ofs.good()){
        std::cerr<<"Fail to open "<<dumpFile<<" for dumping"<<std::endl;
        return EXIT_FAILURE;
    }

    auto* runManager = new G4RunManager();
    runManager->SetUserInitialization(new FTFP_BERT);

    Json::Value jvID;
    Json::Value jvParticle;
    Json::StreamWriterBuilder builder;
    const std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());

    G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleTable::G4PTblDicIterator* theParticleIterator = particleTable->GetIterator();
    theParticleIterator->reset();
    while ((*theParticleIterator)())
    {
        G4ParticleDefinition *theP = theParticleIterator->value();
        if (theP)
        {
            jvParticle["mass"] = G4String(G4BestUnit(theP->GetPDGMass(), "Energy"));
            jvParticle["width"] = G4String(G4BestUnit(theP->GetPDGWidth(), "Energy"));
            jvParticle["charge"] = G4String(G4BestUnit(theP->GetPDGCharge(), "Electric charge"));
            jvParticle["spin"] = theP->GetPDGSpin();
            jvParticle["type"] = theP->GetParticleType();
            jvParticle["lifetime"] = G4String(G4BestUnit(theP->GetPDGLifeTime(),"Time"));
            jvParticle["PDGcode"] = theP->GetPDGEncoding();
            jvID["Name"] = theP->GetParticleName();
            jvID["Property"] = jvParticle;
            writer->write(jvID, &ofs);
        }
    }
     ofs.flush();
     ofs.close();

    //particleTable->DumpTable();
     delete runManager;

     return EXIT_SUCCESS;
}
