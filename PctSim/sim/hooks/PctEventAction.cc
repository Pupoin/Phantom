#include "PctEventAction.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4TrajectoryContainer.hh"
#include "G4VVisManager.hh"
#include "G4ios.hh"
#include "PctRawWriter.hh"
#include "G4VTrajectoryPoint.hh"
#include "G4AutoLock.hh"


PctEventAction::PctEventAction()
 : G4UserEventAction()
{
  //dataI.reserve(500);
  //dataD.reserve(50000);
}


PctEventAction::~PctEventAction()
{;}


void PctEventAction::BeginOfEventAction(const G4Event* evt)
{
  G4cout <<"PctEventAction::"<<__FUNCTION__ << "::EVENT ID=" << evt->GetEventID() << G4endl;

}

void PctEventAction::EndOfEventAction(const G4Event *evt)
{
  G4cout <<"PctEventAction::"<<__FUNCTION__ << "::EVENT ID=" << evt->GetEventID() << G4endl;
}
