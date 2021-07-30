
// $Id$
//
/// \file EventAction.hh
/// \brief Definition of the EventAction class

#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "G4EventManager.hh"
#include "RootManager/RootManager.h"
#include "globals.hh"

#include <fstream>

class EventAction : public G4UserEventAction
{
  public:
    explicit EventAction();
    ~EventAction() override;

    void  BeginOfEventAction(const G4Event* ) override;
    void    EndOfEventAction(const G4Event* ) override;

  private:
    G4int  fPrintModulo;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
