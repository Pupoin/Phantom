#pragma once

#include "PctSdTrackerHit.hh"
#include "G4VSensitiveDetector.hh"
#include <vector>

class PctSdTracker : public G4VSensitiveDetector
{
public:
    PctSdTracker(const G4String &name, const G4String& collName);
    ~PctSdTracker() override;

    // methods from base class
    void Initialize(G4HCofThisEvent *hitCollection) override;

    G4bool ProcessHits(G4Step *step, G4TouchableHistory *history) override;

    void EndOfEvent(G4HCofThisEvent *hitCollection) override;

private:
    //G4String sd_name;
    //G4int copyNo{}; // replical number of PV
    //G4int hit_no{};

    PctSdTrackerHitsCollection* fHitsCollection;
};
