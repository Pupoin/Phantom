

#include "UserAction/PrimaryGeneratorAction.h"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4GeneralParticleSource.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PrimaryGeneratorAction::PrimaryGeneratorAction() : G4VUserPrimaryGeneratorAction() {
    // default generator is general particle siource.
    fCurrentGenerator = fGPS = new G4GeneralParticleSource();

    fParticleGun = new G4ParticleGun();

    fGentypeMap["particleGun"] = fParticleGun;
    fGentypeMap["GPS"] = fGPS;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PrimaryGeneratorAction::~PrimaryGeneratorAction() {
    delete fCurrentGenerator;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent) {

    if (fCurrentGenerator)
        fCurrentGenerator->GeneratePrimaryVertex(anEvent);
    else
        G4Exception("PrimaryGeneratorAction::GeneratePrimaries",
                    "PrimaryGeneratorAction001", FatalException,
                    "generator is not instanciated.");

//    anEvent->GetPrimaryVertex()->Print();
}

