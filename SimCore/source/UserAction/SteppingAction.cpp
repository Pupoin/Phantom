
#include <iostream>

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

double all = 0;
void SteppingAction::UserSteppingAction(const G4Step *aStep)
{

    // Only select initial track
    if (aStep->GetTrack()->GetTrackID() == 1) {

//        if (aStep->GetPostStepPoint()->GetPosition()[2] >= 25 * CLHEP::mm &&
//            aStep->GetPostStepPoint()->GetPosition()[2] <= 30 * CLHEP::mm) {
////            std::cout << aStep->GetPostStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume()->GetMaterial()
////                      << std::endl;
//            std::cout << std::fixed;
//            std::cout << aStep->GetPreStepPoint()->GetPosition()[2] << ", ";
//            std::cout << aStep->GetPostStepPoint()->GetPosition()[2] << ": " << aStep->GetTotalEnergyDeposit()
//                      << std::endl;
//        }

        pRootMng->FillSimStep(pControl->ParticleStep_Name, aStep);
        all += aStep->GetTotalEnergyDeposit();
        G4cout << __LINE__ << " every step: " << aStep->GetTotalEnergyDeposit() << " all: " << all<< G4endl;
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


