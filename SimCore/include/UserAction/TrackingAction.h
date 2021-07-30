
#ifndef TrackingAction_h
#define TrackingAction_h 1

#include "G4UserTrackingAction.hh"
#include "globals.hh"

#include "RootManager/RootManager.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4Track;

/// Tracking action class

class TrackingAction : public G4UserTrackingAction {
public:
    explicit TrackingAction();

    ~TrackingAction() override;

    void PreUserTrackingAction(const G4Track *aTrack) override;

    void PostUserTrackingAction(const G4Track *aTrack) override;

private:

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
