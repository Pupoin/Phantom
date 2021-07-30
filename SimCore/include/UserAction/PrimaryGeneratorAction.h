

#ifndef _PRIMARY_GENERATOR_ACTION_H
#define _PRIMARY_GENERATOR_ACTION_H

#include <map>
#include "G4VPrimaryGenerator.hh"
#include "globals.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
public:
    PrimaryGeneratorAction();

    ~PrimaryGeneratorAction() override;

    void GeneratePrimaries(G4Event *anEvent) override;

private:
    G4ParticleGun *fParticleGun;
    G4VPrimaryGenerator *fGPS;

    G4VPrimaryGenerator *fCurrentGenerator;
    std::map<G4String, G4VPrimaryGenerator *> fGentypeMap;
};

#endif
