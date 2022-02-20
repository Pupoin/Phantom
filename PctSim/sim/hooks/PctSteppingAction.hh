//created by hujf@scnu.edu.cn @2021.10.22
#ifndef PctSteppingAction_h
#define PctSteppingAction_h 1

#include "G4UserSteppingAction.hh"

//
class PctSteppingAction : public G4UserSteppingAction
{
  public:
    PctSteppingAction();
    virtual ~PctSteppingAction();

    virtual void UserSteppingAction(const G4Step*);
};

#endif

