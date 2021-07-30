
#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"

#include "RootManager/RootManager.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4Step;

/// Run action class

class SteppingAction : public G4UserSteppingAction {
public:
    explicit SteppingAction();

    ~SteppingAction() override;

    void UserSteppingAction(const G4Step *aStep) override;

private:

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
