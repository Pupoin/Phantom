#pragma once

#include "PctSdEcalHit.hh"
#include "G4VSensitiveDetector.hh"
#include <vector>

class PctSdEcal : public G4VSensitiveDetector
{
public:
    PctSdEcal(const G4String &name, const G4String& collName);
    ~PctSdEcal() override;

    // methods from base class
    void Initialize(G4HCofThisEvent *hitCollection) override;

    G4bool ProcessHits(G4Step *step, G4TouchableHistory *history) override;

    void EndOfEvent(G4HCofThisEvent *hitCollection) override;

private:
    //G4String sd_name;
    //G4int copyNo{}; // replical number of PV
    //G4int hit_no{};

    PctSdEcalHitsCollection* fHitsCollection;
};
