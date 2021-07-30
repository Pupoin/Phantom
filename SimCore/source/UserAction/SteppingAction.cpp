
#include "UserAction/SteppingAction.h"
#include "RootManager/RootManager.h"
#include "Control/Control.h"

#include "G4Step.hh"
#include "G4EventManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4OpticalPhoton.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction() : G4UserSteppingAction() {
}


SteppingAction::~SteppingAction() = default;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step *aStep) {
    G4StepPoint *prev = aStep->GetPreStepPoint();
    G4StepPoint *post = aStep->GetPostStepPoint();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


