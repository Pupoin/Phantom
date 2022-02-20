
#pragma once

//===================================================
//type B run:
//save data per step for each particle into a RAW file
//===================================================

#include "G4Run.hh"
#include "G4Event.hh"
#include "PctDataType.hh"

class  PctIdealRunB : public G4Run
{
private:
    /* data */
    PCT::READER::PctEvent pctevt;
    std::vector<G4int> databuff;
    
public:
     PctIdealRunB ();
    ~PctIdealRunB ();

    virtual void RecordEvent(const G4Event *);
    virtual void Merge(const G4Run *);
};
