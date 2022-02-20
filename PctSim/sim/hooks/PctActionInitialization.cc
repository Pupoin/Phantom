#include "PctActionInitialization.hh"
#include "PctPrimaryGeneratorAction.hh"
#include "PctRunAction.hh"
#include "PctEventAction.hh"
#include "PctTrackingAction.hh"
#include "PctSteppingAction.hh"
#include "G4Threading.hh"


PctActionInitialization::PctActionInitialization()
{
	//G4cout<<__FUNCTION__<<G4endl;
}


PctActionInitialization::~PctActionInitialization()
{;}


void PctActionInitialization::Build() const
{
	//G4cout << __FUNCTION__ << "::" << G4Threading::G4GetThreadId() << G4endl;

	//
	SetUserAction(new PctPrimaryGeneratorAction);
	//
	SetUserAction(new PctRunAction);
	//
	//SetUserAction(new PctEventAction);
	//
	SetUserAction(new PctTrackingAction);
	//
	//SetUserAction(new PctSteppingAction);
	//
}


void PctActionInitialization::BuildForMaster() const
{
	//
	SetUserAction(new PctRunAction());
	//G4cout << __FUNCTION__ << "::" << G4Threading::G4GetThreadId() << G4endl;
}
