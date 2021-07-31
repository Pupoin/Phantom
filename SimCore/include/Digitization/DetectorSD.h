
/// \file DetectorSD.h
/// \brief Definition of the DetectorSD class

#ifndef DetectorSD_h
#define DetectorSD_h 1

#include "G4VSensitiveDetector.hh"
#include "RootManager/RootManager.h"

#include "DataIO/PHit.h"

#include <vector>
#include <iterator>

enum class SDType {
    NotDefined, Tracker, Calorimeter
};

/// Calorimeter sensitive detector class
///
/// In Initialize(), it creates one hit for each calorimeter layer and one more
/// hit for accounting the total quantities in all layers.
///
/// The values are accounted in hits in ProcessHits() function which is called
/// by Geant4 kernel at each step.

class DetectorSD : public G4VSensitiveDetector {
public:
    DetectorSD(const G4String &name, SDType sdType);

    ~DetectorSD() override;

    // methods from base class
    void Initialize(G4HCofThisEvent *hitCollection) override;

    G4bool ProcessHits(G4Step *step, G4TouchableHistory *history) override;

    void EndOfEvent(G4HCofThisEvent *hitCollection) override;

private:

    SDType fType;
    G4String sd_name;
    G4int copyNo{}; // replical number of PV

    vector<PHit *> fSimHitVec;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

