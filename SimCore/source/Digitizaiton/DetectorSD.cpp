/// \file DetectorSD.cc
/// \brief Implementation of the DetectorSD class

#include "RootManager/RootManager.h"
#include "Digitization/DetectorSD.h"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorSD::DetectorSD(const G4String &name, SDType sdType
) : G4VSensitiveDetector(name) {
    fType = sdType;
    sd_name = name;

    pRootMng->bookCollection(name);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorSD::~DetectorSD() {
    fSimHitVec.clear();
    fSimHitVec.shrink_to_fit();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorSD::Initialize(G4HCofThisEvent *) {
    if (fType == SDType::Calorimeter) {

        if (!fSimHitVec.empty()) {
            fSimHitVec.clear();
        }

        G4ThreeVector arr;
        if (sd_name == pControl->scintillator_name) arr = pControl->scintillator_arrangement;
        else if (sd_name == pControl->telescope_name) arr = pControl->telescope_arrangement;

        for (int i = 0; i < arr.x() * arr.y() * arr.z(); i++)
            fSimHitVec.push_back(new PCTXData());

    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool DetectorSD::ProcessHits(G4Step *step,
                               G4TouchableHistory *) {
    // energy deposit
    G4double edep = step->GetTotalEnergyDeposit();

    // step length
    G4double stepLength = 0.;
    if (step->GetTrack()->GetDefinition()->GetPDGCharge() != 0.) {
        stepLength = step->GetStepLength();
    }

    if (edep == 0. && stepLength == 0.) return false;

    auto *touchable = (G4TouchableHistory *) (step->GetPreStepPoint()->GetTouchable());

    // Get calorimeter cell id
    copyNo = touchable->GetReplicaNumber(1); // for calo, depth=1

    // Get hit accounting data for this cell
    PCTXData *hit;
    if (fType == SDType::Tracker) hit = new PCTXData();
    else hit = fSimHitVec[copyNo];

    // Calculate the center position of this cell
    G4ThreeVector origin(0., 0., 0.);
    G4ThreeVector CellPosition = step->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->
            GetTopTransform().Inverse().TransformPoint(origin);
    G4ThreeVector HitPoint = step->GetPreStepPoint()->GetPosition();

    // Calculate Energy Deposition from EM or Hadron
    G4int PDG = step->GetTrack()->GetDefinition()->GetPDGEncoding();
    G4double E_EM = 0;
    G4double E_Had = 0;
    // electron, positron, gamma
    if (PDG == 11 || PDG == -11 || PDG == 22) E_EM = edep;
    else E_Had = edep;

    // Add values
    hit->setCellId(copyNo + 1); // replica start from 0 in DetectorConstruction
    hit->setX(CellPosition.x() / mm); // mm
    hit->setY(CellPosition.y() / mm); // mm
    hit->setZ(CellPosition.z() / mm); // mm

    // Truth Info
    PCTTruth truth = {(E_EM + E_Had) / MeV, step->GetPostStepPoint()->GetGlobalTime() / ns};
    hit->setTruth(truth);

    if (fType == SDType::Tracker) {
        hit->setDetector(nTracker);
        hit->setCellId(touchable->GetReplicaNumber(0) + 1);
        pRootMng->FillSimHit(sd_name, hit);
    }

    return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorSD::EndOfEvent(G4HCofThisEvent *) {
    if (fType == SDType::Calorimeter) {
        for (auto sim_hit : fSimHitVec) {
            if (sim_hit->getTruth().E >= 1e-10) {
                sim_hit->setDetector(nECAL);
                pRootMng->FillSimHit(sd_name, sim_hit);
            } else
                delete sim_hit;
        }
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
