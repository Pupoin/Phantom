
#pragma once

#include "G4Run.hh"
#include "G4Event.hh"
#include "G4PhysicsVector.hh"

//===================================================
//type full run:
//save data per step for each particle into a ROOT file
//save hits in each detector into a ROOT file
//===================================================

#include "PctDataType.hh"

class  PctFullRun : public G4Run
{
private:
    /* data */
    //PCT::WRITER::PctEvent pctevt;
    std::vector<G4int> databuff;// a serialized stream with integers

public:
     PctFullRun ();
    ~PctFullRun ();

    virtual void RecordEvent(const G4Event *);
    virtual void Merge(const G4Run *);

};
