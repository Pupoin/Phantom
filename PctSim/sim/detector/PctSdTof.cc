
#include "PctSdTof.hh"
#include "PctSdTofHit.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"
#include "PctSharedCode.hh"

//refer to example basic/B4c
PctSdTof::PctSdTof(const G4String &name, const G4String& hitCollName)
    : G4VSensitiveDetector(name), fHitsCollection(NULL)
{
    collectionName.insert(hitCollName);
}

PctSdTof::~PctSdTof()
{
}

void PctSdTof::Initialize(G4HCofThisEvent * hce)
{
  // Create hits collection
  fHitsCollection
    = new PctSdTofHitsCollection(SensitiveDetectorName, collectionName[0]);

  // Add this collection in hce
  auto hcID
    = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection(/*Encoding::CreateCollectionID(Encoding::CID_TOF, hcID)*/hcID, fHitsCollection );

}

G4bool PctSdTof::ProcessHits(G4Step *step,
                             G4TouchableHistory *)
{
    // energy deposit
    G4double edep = step->GetTotalEnergyDeposit();
    if (edep == 0.)
        return false;

    G4StepPoint* preStepPoint = step->GetPreStepPoint();
    G4TouchableHistory* touchable = (G4TouchableHistory*)(preStepPoint->GetTouchable());
    G4int copyNo = touchable->GetVolume()->GetCopyNo();
    //G4int copyNo = touchable->GetReplicaNumber(1);
    G4double hit_time = preStepPoint->GetGlobalTime()/ns;//time
    G4double hit_edep = step->GetTotalEnergyDeposit()/keV;//energy deposition
    G4ThreeVector hit_posi = preStepPoint->GetPosition()/mm;//position
 
    // check if this finger already has a hit
    G4int ix = -1;
    for (G4int i=0;i<fHitsCollection->entries();i++)
    {
      if ((*fHitsCollection)[i]->GetID()==copyNo)
      {
        ix = i;
        break;
      }
    }
 
    PctSdTofHit* hit;
    if (ix>=0) // if it has, then take the earlier time
    {
      hit = (*fHitsCollection)[ix];
      hit->Add(copyNo, hit_edep, hit_time, hit_posi);
    }
    else // if not, create a new hit and set it to the collection
    {
      hit = new PctSdTofHit();
      hit->Add(copyNo, hit_edep, hit_time, hit_posi);
      fHitsCollection->insert(hit);
    }

    // step length
    /*G4double stepLength = 0.;
    if (step->GetTrack()->GetDefinition()->GetPDGCharge() != 0.)
    {
        stepLength = step->GetStepLength();
    }
    auto *touchable = (G4TouchableHistory *)(step->GetPreStepPoint()->GetTouchable());

    // Get calorimeter cell id
    copyNo = touchable->GetReplicaNumber(1); // for calo, depth=1

    // Get hit accounting data for this cell
    PCTXData *hit;
    if (fType == SDType::Tracker)
        hit = new PCTXData();
    else
        hit = fSimHitVec[copyNo];

    // Calculate the center position of this cell
    G4ThreeVector origin(0., 0., 0.);
    G4ThreeVector CellPosition = step->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().Inverse().TransformPoint(origin);
    G4ThreeVector HitPoint = step->GetPreStepPoint()->GetPosition();

    // Calculate Energy Deposition from EM or Hadron
    G4int PDG = step->GetTrack()->GetDefinition()->GetPDGEncoding();
    G4double E_EM = 0;
    G4double E_Had = 0;
    // electron, positron, gamma
    if (PDG == 11 || PDG == -11 || PDG == 22)
        E_EM = edep;
    else
        E_Had = edep;

    // Add values
    hit->setCellId(copyNo + 1);       // replica start from 0 in DetectorConstruction
    hit->setX(CellPosition.x() / mm); // mm
    hit->setY(CellPosition.y() / mm); // mm
    hit->setZ(CellPosition.z() / mm); // mm

    // Truth Info
    PCTTruth truth = {(E_EM + E_Had) / MeV, step->GetPostStepPoint()->GetGlobalTime() / ns,
                      HitPoint.x() / mm, HitPoint.y() / mm, HitPoint.z() / mm, step->GetTrack()->GetTrackID()};
    hit->addTruth(truth);
    hit->setE(hit->getE() + truth.E);

    double T_weighted = truth.T * (truth.E / (hit->getE())) + hit->getT() * ((hit->getE() - truth.E) / hit->getE());
    hit->setT(T_weighted);
    if (fType == SDType::Tracker)
    {
        hit->setDetector(nTracker);
        hit->setCellId(touchable->GetReplicaNumber(0) + 1);
        pRootMng->FillSimHit(sd_name, hit);
    }
    */

    return true;
}

void PctSdTof::EndOfEvent(G4HCofThisEvent * hce)
{
}
