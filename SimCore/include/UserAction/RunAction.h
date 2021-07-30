
/// \file RunAction.hh
/// \brief Definition of the RunAction class

#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "RootManager/RootManager.h"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4Run;

/// Run action class

class RunAction : public G4UserRunAction
{
  public:
    explicit RunAction();
    ~RunAction() override;

    void BeginOfRunAction(const G4Run* run) override;
    void   EndOfRunAction(const G4Run* run) override;

  private:
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
