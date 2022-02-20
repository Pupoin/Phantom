#pragma once
//===================================================
//PctDigiRun
//save digis in detector into a RAW file.
//===================================================

#include "G4Run.hh"
#include "G4Event.hh"
#include "G4PhysicsVector.hh"
#include "PctRawWriter.hh"

//generate digitizaiton signals 
//and collect them

class  PctDigiRun : public G4Run
{
private:
    /* data */
    std::vector<G4int> databuff;
public:
     PctDigiRun ();
    ~PctDigiRun ();

    virtual void RecordEvent(const G4Event *);
    virtual void Merge(const G4Run *);
};
