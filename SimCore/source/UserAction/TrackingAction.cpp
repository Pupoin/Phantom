
#include "UserAction/TrackingAction.h"
#include "RootManager/RootManager.h"
#include "Control/Control.h"

#include "G4Track.hh"
#include "G4TrackingManager.hh"
#include "G4Event.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrackingAction::TrackingAction() : G4UserTrackingAction() {
}

TrackingAction::~TrackingAction() = default;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrackingAction::PreUserTrackingAction(const G4Track *aTrack) {
    /* Initialize Filter */

    auto pdg = aTrack->GetParticleDefinition()->GetPDGEncoding();
    auto energy = aTrack->GetTotalEnergy();
    auto kin_energy = aTrack->GetKineticEnergy();


    G4double pm = sqrt(aTrack->GetMomentum()[0] * aTrack->GetMomentum()[0] +
                       aTrack->GetMomentum()[1] * aTrack->GetMomentum()[1] +
                       aTrack->GetMomentum()[2] * aTrack->GetMomentum()[2]);

    if (pControl->save_all_mcp || (aTrack->GetTrackID() == 1
                                   || pm >= 1. * GeV
                                   || (kin_energy >= 1. * GeV && kin_energy <= 10. * GeV)
                                   || abs(pdg) == 13   // Muon
                                   || abs(pdg) == 111  // Pion0
                                   || abs(pdg) == 211  // Pion+-
                                   || abs(pdg) == 321  // Kaon+-
                                   || abs(pdg) == 2212 // proton
                                   || abs(pdg) == 2112 // neutron
                                   || abs(pdg) == 14   // muon neutrino
                                   || abs(pdg) == 12   // electron neutrino
                                   || (abs(pdg) >= 100 &&  abs(pdg) <= 10000) // inclusive hadrons

    )) {

    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrackingAction::PostUserTrackingAction(const G4Track *aTrack) {

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
