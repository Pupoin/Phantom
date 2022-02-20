#include "PctSteppingAction.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Material.hh"
#include "G4StepPoint.hh"
#include "G4SteppingManager.hh"


PctSteppingAction::PctSteppingAction()
 : G4UserSteppingAction()
{
}


PctSteppingAction::~PctSteppingAction()
{;}


void PctSteppingAction::UserSteppingAction(const G4Step* aStep)
{
//fpSteppingManager;
  //const G4Track* trk = aStep->GetTrack();
  //G4cout << "PreStep " << aStep->GetPreStepPoint()->GetPosition()
  //       << " -- PostStep " << aStep->GetPostStepPoint()->GetPosition()
  //       << " >>> Material " << trk->GetMaterial()->GetName() << G4endl;
}

