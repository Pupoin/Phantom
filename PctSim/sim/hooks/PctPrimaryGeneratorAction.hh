//created by hujf@scnu.edu.cn @2021.10.22
#ifndef PctPrimaryGeneratorAction_h
#define PctPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class G4VPrimaryGenerator;
class G4GeneralParticleSource;
class G4ParticleGun;
class G4Event;

//
class PctPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    PctPrimaryGeneratorAction();    
    virtual ~PctPrimaryGeneratorAction();

  public:
    virtual void GeneratePrimaries(G4Event*);

  private:
    G4ParticleGun *fParticleGun;
    G4GeneralParticleSource *fGPS;

    G4VPrimaryGenerator *fCurrentGenerator;
    std::map<G4String, G4VPrimaryGenerator *> fGentypeMap;

};

#endif
