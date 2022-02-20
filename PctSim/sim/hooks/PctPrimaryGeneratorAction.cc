#include "PctPrimaryGeneratorAction.hh"
#include "Randomize.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"    
#include "G4UnitsTable.hh"


	PctPrimaryGeneratorAction::PctPrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(),
	fParticleGun(0), fGPS(0)
{
	G4int n_particle = 1;
	fParticleGun = new G4ParticleGun(n_particle);
	fGPS = new G4GeneralParticleSource();
	fGentypeMap["GUN"] = fParticleGun;
	fGentypeMap["GPS"] = fGPS;
	fCurrentGenerator = fGPS;
//G4cout<<__FUNCTION__<<G4endl;
}


PctPrimaryGeneratorAction::~PctPrimaryGeneratorAction()
{
	delete fParticleGun;
	delete fGPS;
}


void PctPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
	fCurrentGenerator->GeneratePrimaryVertex(anEvent);
	//G4cout<<__FUNCTION__<<anEvent->GetNumberOfPrimaryVertex()<<G4endl;
	//for(int iv=0;iv<anEvent->GetNumberOfPrimaryVertex();++iv){
	//	G4PrimaryVertex* pv = anEvent->GetPrimaryVertex(iv);
	//	G4cout<<"The "<<iv<<"th vertex, position="<<G4BestUnit(pv->GetPosition(), "Length")<<", time="<<G4BestUnit(pv->GetT0(),"Time")<<G4endl;
	//}
}

