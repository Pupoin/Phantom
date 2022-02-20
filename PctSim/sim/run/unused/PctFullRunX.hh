#pragma once

#include "G4Run.hh"
#include "G4Event.hh"
#include "G4PhysicsVector.hh"
#include "DataIO/PCTEvent.h"

//===================================================
//type full run:
//save data per step for each particle into a ROOT file
//save hits in each detector into a ROOT file
//===================================================

#include "PctDataType.hh"

class  PctFullRunX : public G4Run
{
private:
    /* data */
    PCTEvent pctevt;

public:
     PctFullRunX ();
    ~PctFullRunX ();

    virtual void RecordEvent(const G4Event *);
    virtual void Merge(const G4Run *);

};
