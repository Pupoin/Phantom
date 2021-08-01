
#include "UserAction/SteppingAction.h"
#include "RootManager/RootManager.h"
#include "Control/Control.h"

#include "G4Step.hh"
#include "G4EventManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4OpticalPhoton.hh"

/*
 * data recorded in root manager:
 * 1. initial proton step
 */


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction() : G4UserSteppingAction() {
}


SteppingAction::~SteppingAction() = default;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step *aStep) {

    // Only select initial track
    if (aStep->GetTrack()->GetTrackID() == 1) {
        pRootMng->FillSimStep(pControl->ParticleStep_Name, aStep);
    }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


